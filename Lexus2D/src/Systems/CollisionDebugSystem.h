#ifndef COLLISIONDEBUGSYSTEM_H
#define COLLISIONDEBUGSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ColliderComponent.h"

class CollisionDebugSystem : public System {
public:
	CollisionDebugSystem() {
		RequireComponent<BoxColliderComponent>();
	}

	void Draw(SDL_Renderer* i_renderer) {

		for (Entity entity : GetSystemEntities()) {
			BoxColliderComponent collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Color debugColor = collider.isTriggered ? triggeredColor : untriggeredColor;

			SDL_SetRenderDrawColor(i_renderer, debugColor.r, debugColor.g, debugColor.b, debugColor.a);
			SDL_RenderDrawRect(i_renderer, &collider.rect);
		}
	}

private:
	SDL_Color triggeredColor = { 255,0,0,1 };
	SDL_Color untriggeredColor = { 0,255,0,1 };
};

#endif