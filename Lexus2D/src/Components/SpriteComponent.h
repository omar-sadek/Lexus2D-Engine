#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>

enum RenderLayer {
	DEFAULT_L = 0,
	BACKGROUND_L = 1,
	ENEMY_L = 2,
	PLAYER_L = 3,
	PROJECTILE_L = 4,
	UI_L = 5
};

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	RenderLayer layer;
	SDL_RendererFlip flip;
	SDL_Rect srcRect;

	SpriteComponent(
		std::string i_assetId = "",
		int i_width = 10, int i_height = 10,
		RenderLayer i_layer = RenderLayer::DEFAULT_L,
		SDL_RendererFlip i_flip = SDL_RendererFlip::SDL_FLIP_NONE,
		int i_srcRectX = 0, int i_srcRectY = 0)
	{
		assetId = i_assetId;
		width = i_width;
		height = i_height;
		layer = i_layer;
		flip = i_flip;
		srcRect = { i_srcRectX, i_srcRectY, width, height };
	}
};
#endif