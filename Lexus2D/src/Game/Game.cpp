#include "Game.h"
#include <glm/glm.hpp>
#include <SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "SceneLoader.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/FrameSwapperSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/CollisionDebugSystem.h"
#include "../Systems/InputControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileSpawnSystem.h"
#include "../Systems/ProjectileBehaviourSystem.h"
#include "../Systems/TextRenderSystem.h"
#include "../Systems/GUIRenderSystem.h"

int Game::WindowWidth;
int Game::WindowHeight;
int Game::MapWidth;
int Game::MapHeight;

Game::Game() {
	isRunning = false;
	isDebugMode = false;
	entityManager = std::make_shared<EntityManager>();
	assetManager = std::make_unique<AssetManager>();
	eventManager = std::make_unique<EventBus>();

	Logger::Warning("Game Constructor Called!");
}

Game::~Game() {
	Logger::Warning("Game Destructor Called!");
}

void Game::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Error("Error initializing SDL.");
		return;
	}

	if (TTF_Init() != 0) {
		Logger::Error("Error initializing TTF");
		return;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	WindowWidth = 1280;
	WindowHeight = 720;

	window = SDL_CreateWindow(
		"Lexus2D GameEngine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WindowWidth, WindowHeight,
		NULL
	);

	if (!window) {
		Logger::Error("Error creating SDL window.");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		Logger::Error("Error creating SDL renderer.");
		return;
	}

	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, WindowWidth, WindowHeight);

	camera.x = 0;
	camera.y = 0;
	camera.w = WindowWidth;
	camera.h = WindowHeight;

	isRunning = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
	entityManager->AddSystem<MovementSystem>();
	entityManager->AddSystem<RenderSystem>();
	entityManager->AddSystem<FrameSwapperSystem>();
	entityManager->AddSystem<CollisionSystem>();
	entityManager->AddSystem<CollisionDebugSystem>();
	entityManager->AddSystem<InputControlSystem>();
	entityManager->AddSystem<CameraMovementSystem>();
	entityManager->AddSystem<ProjectileSpawnSystem>();
	entityManager->AddSystem<ProjectileBehaviourSystem>();
	entityManager->AddSystem<TextRenderSystem>();
	entityManager->AddSystem<GUIRenderSystem>();

	entityManager->GetSystem<ProjectileSpawnSystem>().InitializeSystem(entityManager);
	
	SceneLoader sceneLoader;
	lua.open_libraries(sol::lib::base, sol::lib::math);
	sceneLoader.LoadScene(lua, entityManager, assetManager, renderer, "Scene1");

	loadEntities();
}

void Game::loadEntities() {

	// Player

	Entity player = entityManager->CreateEntity();
	player.SetTag(Tag::PLAYER_T);
	player.AddComponent<TransformComponent>(glm::vec2(100.0, 200.0), glm::vec2(2, 2), 0.0);
	player.AddComponent<RigidBodyComponent>();
	player.AddComponent<SpriteComponent>("character-sprites", 16, 16, RenderLayer::ENEMY_L, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	player.AddComponent<FrameSwapperComponent>(4, 0.15, true, true);
	player.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0));
	player.AddComponent<InputControlledComponent>(glm::vec2(0, -70), glm::vec2(70, 0), glm::vec2(0, 70), glm::vec2(-70, 0));
	player.AddComponent<ProjectileSpawnerComponent>(200, glm::vec2(0, 0), 500, 10000, 10, false, true);
	player.AddComponent<CameraFollowComponent>();

	// UI

	Entity face = entityManager->CreateEntity();
	face.AddComponent<TransformComponent>(glm::vec2(Game::WindowWidth - 144, Game::WindowHeight - 144), glm::vec2(1.0, 1.0), 0.0);
	face.AddComponent<SpriteComponent>("character-ui-sprites", 144, 144, RenderLayer::UI_L);
	face.AddComponent<FrameSwapperComponent>(4, 2, true);

	// Text

	Entity label = entityManager->CreateEntity();
	SDL_Color labelColor = { 255,255,0 };
	label.AddComponent<TextComponent>(glm::vec2(Game::WindowWidth / 2 - 40, 5), "LEXUS 2D", "charriot-font_l", labelColor, true);
}

/// <summary>Used to update the game every frame.</summary>
void Game::Update() {

#pragma region FRAME CAP
	int delayTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPrevFrame);
	if (delayTime > 0 && delayTime <= MILLISECS_PER_FRAME)
		SDL_Delay(delayTime);
#pragma endregion FRAME CAP

	double deltaTime = (SDL_GetTicks() - millisecsPrevFrame) / 1000.0;

	millisecsPrevFrame = SDL_GetTicks();

	eventManager->Reset();

	entityManager->GetSystem<InputControlSystem>().SubscribeToEvent(eventManager);
	entityManager->GetSystem<ProjectileSpawnSystem>().SubscribeToEvent(eventManager);

	entityManager->Update();

	entityManager->GetSystem<MovementSystem>().Update(deltaTime);
	entityManager->GetSystem<FrameSwapperSystem>().Update();
	entityManager->GetSystem<CollisionSystem>().Update(eventManager, camera);
	entityManager->GetSystem<CameraMovementSystem>().Update(camera);
	entityManager->GetSystem<ProjectileSpawnSystem>().Update();
	entityManager->GetSystem<ProjectileBehaviourSystem>().Update();
}

/// <summary>Starts the game loop.</summary>
void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

/// <summary>Used to get inputs and fire events.</summary>
void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		ImGuiIO& io = ImGui::GetIO();
		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		io.MousePos = ImVec2(mouseX, mouseY);
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			eventManager->DispatchEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			if (sdlEvent.key.keysym.sym == SDLK_TAB) {
				isDebugMode = !isDebugMode;
			}
			break;
		}
	}
}

/// <summary>Used for rendering.</summary>
void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	entityManager->GetSystem<RenderSystem>().Draw(renderer, assetManager, camera);
	entityManager->GetSystem<TextRenderSystem>().Draw(renderer, assetManager, camera);
	if (isDebugMode) {
		entityManager->GetSystem<CollisionDebugSystem>().Draw(renderer);
		entityManager->GetSystem<GUIRenderSystem>().Draw(entityManager);
	}
	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}