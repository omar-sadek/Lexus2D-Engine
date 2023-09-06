#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL.h>
#include <algorithm>
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetManager.h"


class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Draw(SDL_Renderer* i_renderer, std::unique_ptr<AssetManager>& i_assetManager, SDL_Rect& i_camera) {
		std::vector<Entity> sortedEntities = GetSystemEntities();
		std::sort(sortedEntities.begin(), sortedEntities.end(), compareByLayer);

		for (Entity entity : sortedEntities) {
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

			//OPTIMIZATION: culling should be made before sorting
			bool isOutSideCamera = (
				transform.position.x + (transform.scale.x * sprite.width) < i_camera.x ||
				transform.position.x > i_camera.x + i_camera.w ||
				transform.position.y + (transform.scale.y * sprite.height) < i_camera.y ||
				transform.position.y > i_camera.y + i_camera.h
				);

			if (isOutSideCamera && !sprite.layer == RenderLayer::UI_L) continue;

			SDL_Rect srcRect = sprite.srcRect;

			SDL_Rect distRect = {
				static_cast<int>(transform.position.x - (sprite.layer == RenderLayer::UI_L ? 0 : i_camera.x)),
				static_cast<int>(transform.position.y - (sprite.layer == RenderLayer::UI_L ? 0 : i_camera.y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			/*SDL_SetRenderDrawColor(i_renderer, 255, 255, 255, 255);
			drawCircle(i_renderer, transform.position.x + (sprite.width * 0.5), transform.position.y + ( sprite.height * 0.5), 3);*/

			SDL_RenderCopyEx(
				i_renderer,
				i_assetManager->GetVisualAsset(sprite.assetId),
				&srcRect,
				&distRect,
				transform.rotation,
				NULL,
				sprite.flip
			);
		}
	}
private:
	static bool compareByLayer(const Entity& a, const Entity& b)
	{
		return a.GetComponent<SpriteComponent>().layer <
			b.GetComponent<SpriteComponent>().layer;
	}

	void drawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
	{
		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}
};


#endif