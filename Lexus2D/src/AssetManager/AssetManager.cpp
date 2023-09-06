#include <SDL_image.h>
#include "./AssetManager.h"
#include "../Logger/Logger.h"

AssetManager::AssetManager() {
	//Logger::Warning("AssetManager constructor called!");
}

AssetManager::~AssetManager() {
	ClearAssets();
	//Logger::Warning("AssetManager destructor called!");
}

void AssetManager::AddTexture(SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath) {
	addVisualAsset(textures, i_renderer, i_assetId, i_filePath);
	Logger::Log("New Texture was added with id: " + i_assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& i_assetId) {
	return textures[i_assetId];
}

void AssetManager::AddTileset(SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath) {
	addVisualAsset(tilesets, i_renderer, i_assetId, i_filePath);
	Logger::Log("New Tileset was added with id: " + i_assetId);
}

SDL_Texture* AssetManager::GetTileset(const std::string& i_assetId) {
	return tilesets[i_assetId];
}

SDL_Texture* AssetManager::GetVisualAsset(const std::string& i_assetId) {
	if (GetTexture(i_assetId) != NULL) return GetTexture(i_assetId);
	else if (GetTileset(i_assetId) != NULL) return GetTileset(i_assetId);

	Logger::Error("AssetManager couldn't find a visual texure with the id: " + i_assetId);
	return NULL;
}

void AssetManager::AddFont(const std::string& i_assetId, const std::string& i_filePath, int i_fontSize) {
	fonts.emplace(i_assetId, TTF_OpenFont(i_filePath.c_str(), i_fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& i_assetId) {
	return fonts[i_assetId];
}

void AssetManager::addVisualAsset(std::map<std::string, SDL_Texture*>& i_assetMap, SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath) {
	SDL_Surface* surface = IMG_Load(i_filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(i_renderer, surface);
	SDL_FreeSurface(surface);

	i_assetMap.emplace(i_assetId, texture);
}

void AssetManager::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}

	for (auto tileset : tilesets) {
		SDL_DestroyTexture(tileset.second);
	}

	for (auto font : fonts) {
		TTF_CloseFont(font.second);
	}

	textures.clear();
	tilesets.clear();
	fonts.clear();
}