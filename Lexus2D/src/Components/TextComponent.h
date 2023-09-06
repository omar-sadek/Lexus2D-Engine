#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>

struct TextComponent {
	glm::vec2 position;
	std::string text;
	std::string assetId;
	SDL_Color color;
	bool isOverlay;

	TextComponent(glm::vec2 i_position = glm::vec2(0), std::string i_text = "", std::string i_assetId = "", const SDL_Color& i_color = { 255,255,255 }, bool i_isOverlay = true) {
		position = i_position;
		text = i_text;
		assetId = i_assetId;
		color = i_color;
		isOverlay = i_isOverlay;
	}
};

#endif