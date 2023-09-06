#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>

class SceneLoader {
public:
	SceneLoader();
	~SceneLoader();
	void LoadScene(sol::state& i_lua, const std::shared_ptr<EntityManager>& i_entityManager, const std::unique_ptr<AssetManager>& i_assetManager, SDL_Renderer* i_renderer, std::string i_sceneName);
private:
	void loadAssets(const std::unique_ptr<AssetManager>& i_assetManager, SDL_Renderer* i_renderer, sol::table& i_scene);
	void loadTilemap(const std::shared_ptr<EntityManager>& i_entityManager, sol::table& i_scene);
};

#endif