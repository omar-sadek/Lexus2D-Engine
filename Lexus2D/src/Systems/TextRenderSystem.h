#ifndef TEXTRENDERSYSTEM_H
#define TEXTRENDERSYSTEM_H

#include <SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include "../Components/TextComponent.h"

class TextRenderSystem : public System {
public:
	TextRenderSystem() {
		RequireComponent<TextComponent>();
	}

	void Draw(SDL_Renderer* i_renderer, std::unique_ptr<AssetManager>& i_assetManager, const SDL_Rect& i_camera) {
		for (Entity entity : GetSystemEntities()) {
			const TextComponent text = entity.GetComponent<TextComponent>();

			SDL_Surface* surface = TTF_RenderText_Blended(i_assetManager->GetFont(text.assetId), text.text.c_str(), text.color);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(i_renderer, surface);

			SDL_FreeSurface(surface);

			int textWidth = 0;
			int textHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

			SDL_Rect distRect = {
				static_cast<int>(text.position.x - (text.isOverlay ? 0 : i_camera.x)),
				static_cast<int>(text.position.y - (text.isOverlay ? 0 : i_camera.y)),
				textWidth, textHeight
			};

			SDL_RenderCopy(i_renderer, texture, NULL, &distRect);

			SDL_DestroyTexture(texture);
		}
	}
};

#endif  
