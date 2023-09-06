#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class AssetManager {
private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, SDL_Texture*> tilesets;
	std::map<std::string, TTF_Font*> fonts;

	void addVisualAsset(std::map<std::string, SDL_Texture*>& i_assetMap, SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath);
public:
	AssetManager();
	~AssetManager();

	void AddTexture(SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath);
	SDL_Texture* GetTexture(const std::string& i_assetId);

	void AddTileset(SDL_Renderer* i_renderer, const std::string& i_assetId, const std::string& i_filePath);
	SDL_Texture* GetTileset(const std::string& i_assetId);

	SDL_Texture* GetVisualAsset(const std::string& i_assetId);

	void AddFont(const std::string& i_assetId, const std::string& i_filePath, int i_fontSize);
	TTF_Font* GetFont(const std::string& i_assetId);

	void ClearAssets();
};

#endif 
