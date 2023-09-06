#ifndef FRAMESWAPPERSYSTEM_H
#define FRAMESWAPPERSYSTEM_H

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/FrameSwapperComponent.h"


class FrameSwapperSystem : public System {
public:
	FrameSwapperSystem() {
		RequireComponent<SpriteComponent>();
		RequireComponent<FrameSwapperComponent>();
	}

	void Update() {
		for (Entity entity : GetSystemEntities()) {
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
			FrameSwapperComponent& swapper = entity.GetComponent<FrameSwapperComponent>();

			if (!swapper.isRunning) continue;

			swapper.currentFrameIndex =
				(int)((SDL_GetTicks() - swapper.startTime) * (1 / swapper.frameTime) / 1000) % swapper.framesCount;
			
			sprite.srcRect.x = swapper.currentFrameIndex * sprite.width;
		}
	}
};

#endif