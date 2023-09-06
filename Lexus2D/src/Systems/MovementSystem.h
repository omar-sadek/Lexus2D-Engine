#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

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

#endif
