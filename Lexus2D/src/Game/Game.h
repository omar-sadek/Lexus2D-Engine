#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	void loadEntities();
	bool isRunning;
	bool isDebugMode;
	int millisecsPrevFrame;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;

	std::shared_ptr<EntityManager> entityManager;
	std::unique_ptr<AssetManager> assetManager;
	std::unique_ptr<EventBus> eventManager;

	sol::state lua;

public:
	Game();
	~Game();
	void Init();
	void Setup();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	static int WindowWidth;
	static int WindowHeight;
	static int MapWidth;
	static int MapHeight;
};

#endif
