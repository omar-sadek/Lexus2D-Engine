# Lexus2D-Engine

A 2D game engine created as a personal project to practice data-oriented design (DOD) and enhance my C++ programming skills. This engine includes a demo to showcase its capabilities, although it's important to note that it's a work in progress and far from perfect, as it's primarily used for learning and experimentation. 

![Preview](https://s11.gifyu.com/images/SgG7q.gif)

## Dependencies

Before you can build and run this 2D engine, make sure you have the following dependencies installed:

- **SDL2**: The engine uses SDL2 for graphics rendering and window management.

- **ImGUI**: ImGUI is integrated for creating in-game debugging tools and user interfaces.

- **glm**: The glm library is employed for math operations, including vector and matrix calculations.

- **Lua**: Lua scripting support is incorporated for game logic and customization.

## Example: Creating an Entity that moves
Creating the Entity:

```cpp
void Game::loadEntities() {
Entity player = entityManager->CreateEntity();
	player.SetTag(Tag::PLAYER_T);
        //transform(position,scale,rotation)
	player.AddComponent<TransformComponent>(glm::vec2(100.0, 200.0), glm::vec2(2, 2), 0.0);
	player.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
}
```
RigidBody Component:

```cpp
struct RigidBodyComponent {
	glm::vec2 velocity;
	double rotationSpeed;

	RigidBodyComponent(glm::vec2 i_velocity = glm::vec2(0.0, 0.0), double i_rotationSpeed = 0.0) {
		velocity = i_velocity;
		rotationSpeed = i_rotationSpeed;
	}
};
```

Creating the MovementSystem:

```cpp
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}


	void Update(double i_deltaTime) {
              //looping through every entity that has all required component
		for (auto entity : GetSystemEntities()) {
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const RigidBodyComponent rigidBody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidBody.velocity.x * i_deltaTime;
			transform.position.y += rigidBody.velocity.y * i_deltaTime;
			transform.rotation += rigidBody.rotationSpeed * i_deltaTime;

			bool isOutsideMap = (
				transform.position.x < 0 ||
				transform.position.y < 0 ||
				transform.position.x > Game::MapWidth ||
				transform.position.y > Game::MapHeight
				);

			if (isOutsideMap && !entity.HasTag(Tag::PLAYER_T))
				entity.Destroy();
		}
	}
};
```

To run the system you need to add to add it to the entity manager and run it

```cpp
void Game::Setup() {
	entityManager->AddSystem<MovementSystem>();
}

void Game::Update() {

#pragma region FRAME CAP
	int delayTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPrevFrame);
	if (delayTime > 0 && delayTime <= MILLISECS_PER_FRAME)
		SDL_Delay(delayTime);
#pragma endregion FRAME CAP

	double deltaTime = (SDL_GetTicks() - millisecsPrevFrame) / 1000.0;

	millisecsPrevFrame = SDL_GetTicks();

	eventManager->Reset();

	entityManager->Update();

	entityManager->GetSystem<MovementSystem>().Update(deltaTime);
}
```

## Implemented Systems:

- Movement System.
- Render System.
- Collision Detection System (90% done).
- Collsion Debug System.
- Scene Loader: importing assets from lua script.
- Frame Swapper (Animation System).
- Input Control System.
- Camera Movement System.
- Projectile Spawner.
- Text Render System.
- GUI Render System: Enemy spawner tool using ImGUI.

# Credits and References

## Art Assets

The artwork used in the demo was created by [Jamie Brownhill](https://ko-fi.com/jamiebrownhill). Please visit the artist's [portfolio](https://www.jamiebrownhill.com/) to explore more of their fantastic work.

## External References

During the development, I found the following resources and references valuable:

- [Nomad Game Engine](https://savas.ca/nomad): A blog post series introducing ECS concepts and their applications in game development.

- [Mix Engine](https://github.com/arvidsson/Mix): An example of a robust ECS-based game engine.

- [Austin Morlan's Game Engine](https://austinmorlan.com/posts/entity_component_system/): Another example of a robust ECS-based game engine.

- [Game Programming Patterns - Entity Component System](https://gameprogrammingpatterns.com/component.html): A chapter from the "Game Programming Patterns" book that provides insights into ECS design.


