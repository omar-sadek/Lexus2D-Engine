#ifndef PROJECTILESPAWNSYSTEM_H
#define PROJECTILESPAWNSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/ProjectileSpawnerComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ColliderComponent.h"

class ProjectileSpawnSystem : public System {
public:
	ProjectileSpawnSystem() {
		RequireComponent<ProjectileSpawnerComponent>();
		RequireComponent<TransformComponent>();
	}

	void InitializeSystem(std::shared_ptr<EntityManager>& i_entityManager) {
		entityManager = i_entityManager;
	}

	void SubscribeToEvent(std::unique_ptr<EventBus>& i_eventManager) {

		i_eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileSpawnSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& i_event) {
		switch (i_event.KeyCode) {
		case SDLK_SPACE:
			for (Entity entity : GetSystemEntities()) {
				ProjectileSpawnerComponent& projectileSpawner = entity.GetComponent<ProjectileSpawnerComponent>();
				if (!projectileSpawner.isAutomatic)
					spawnProjectile(entity, projectileSpawner, true);
			}
			break;
		}
	}

	void Update() {
		for (Entity entity : GetSystemEntities()) {
			ProjectileSpawnerComponent& projectileSpawner = entity.GetComponent<ProjectileSpawnerComponent>();

			if (projectileSpawner.isAutomatic && (SDL_GetTicks() - projectileSpawner.lastSpawnedTime > projectileSpawner.frequency))
				spawnProjectile(entity, projectileSpawner, false);
		}
	}

private:

	void spawnProjectile(Entity& i_entity, ProjectileSpawnerComponent& i_projectileSpawner, bool i_overrideDirection) {
		const TransformComponent transform = i_entity.GetComponent<TransformComponent>();
		glm::vec2 projectilePos = transform.position;
		if (i_entity.HasComponent<SpriteComponent>()) {
			const SpriteComponent sprite = i_entity.GetComponent<SpriteComponent>();
			projectilePos.x += (transform.scale.x * sprite.width / 2);
			projectilePos.y += (transform.scale.y * sprite.height / 2);
		}

		Entity projectileEntity = entityManager->CreateEntity();
		projectileEntity.SetGroup(Group::PROJECTILES_G);

		projectileEntity.AddComponent<TransformComponent>(projectilePos);

		if (i_overrideDirection && i_entity.HasComponent<RigidBodyComponent>())
		{
			const RigidBodyComponent rigidBody = i_entity.GetComponent<RigidBodyComponent>();
			i_projectileSpawner.direction = glm::normalize(rigidBody.velocity);
		}

		glm::vec2 projectileVelocity = glm::normalize(i_projectileSpawner.direction);
		projectileVelocity *= i_projectileSpawner.speed;
		projectileEntity.AddComponent<RigidBodyComponent>(projectileVelocity);

		projectileEntity.AddComponent<SpriteComponent>("projectile-image", 4, 4, RenderLayer::PROJECTILE_L);
		projectileEntity.AddComponent<BoxColliderComponent>(4, 4);
		projectileEntity.AddComponent<ProjectileComponent>(i_projectileSpawner.duration, i_projectileSpawner.damage, i_projectileSpawner.isFriendly);

		i_projectileSpawner.lastSpawnedTime = SDL_GetTicks();
	}
};

#endif