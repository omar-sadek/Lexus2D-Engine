#ifndef PROJECTILEBEHAVIOURSYSTEM_H
#define PROJECTILEBEHAVIOURSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileBehaviourSystem : public System {
public:
	ProjectileBehaviourSystem() {
		RequireComponent<ProjectileComponent>();
	}

	void Update() {
		for (Entity entity : GetSystemEntities()) {
			ProjectileComponent projectile = entity.GetComponent<ProjectileComponent>();

			if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
				entity.Destroy();
			}
		}
	}
};

#endif
