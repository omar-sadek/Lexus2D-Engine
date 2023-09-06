#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include <SDL_image.h>

#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"

class CameraMovementSystem : public System
{
public:
	CameraMovementSystem() {
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(SDL_Rect& i_camera) {
		for (Entity entity : GetSystemEntities()) {
			TransformComponent transform = entity.GetComponent<TransformComponent>();

			if (transform.position.x + (i_camera.w / 2) < Game::MapWidth)
				i_camera.x = transform.position.x - (Game::WindowWidth / 2);
			if (transform.position.y + (i_camera.h / 2) < Game::MapHeight)
				i_camera.y = transform.position.y - (Game::WindowHeight / 2);

			i_camera.x = i_camera.x < 0 ? 0 : i_camera.x;
			i_camera.y = i_camera.y < 0 ? 0 : i_camera.y;

			i_camera.x = i_camera.x > i_camera.w ? i_camera.w : i_camera.x;
			i_camera.y = i_camera.y > i_camera.h ? i_camera.h : i_camera.y;
		}
	}
};

#endif 
