#include "SceneLoader.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <sol/sol.hpp>

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ColliderComponent.h"

SceneLoader::SceneLoader() {

}

SceneLoader::~SceneLoader() {

}

void SceneLoader::LoadScene(sol::state& i_lua, const std::shared_ptr<EntityManager>& i_entityManager, const std::unique_ptr<AssetManager>& i_assetManager, SDL_Renderer* i_renderer, std::string i_sceneName) {
	sol::load_result script = i_lua.load_file("./assets/scripts/" + i_sceneName + ".lua");

	if (false == script.valid()) {
		sol::error error = script;
		std::string errorMsg = error.what();
		Logger::Error("Error loading the scene with the following error message:\n (" + errorMsg + ")");
		return;
	}

	i_lua.script_file("./assets/scripts/" + i_sceneName + ".lua");

	sol::table scene = i_lua["Scene"];

	loadAssets(i_assetManager, i_renderer, scene);

	loadTilemap(i_entityManager, scene);
}

void SceneLoader::loadAssets(const std::unique_ptr<AssetManager>& i_assetManager, SDL_Renderer* i_renderer, sol::table& i_scene) {
	sol::table assets = i_scene["assets"];

	int i = 0;
	while (true) {
		sol::optional<sol::table> hasAsset = assets[i];
		if (hasAsset == sol::nullopt)
			break;
		sol::table asset = assets[i];
		std::string assetType = asset["type"];

		if (assetType == "texture") {
			i_assetManager->AddTexture(i_renderer, asset["id"], asset["file"]);
		}
		else if (assetType == "font") {
			i_assetManager->AddFont(asset["id"], asset["file"], asset["font_size"]);
		}
		i++;
	}
}

void SceneLoader::loadTilemap(const std::shared_ptr<EntityManager>& i_entityManager, sol::table& i_scene) {
	sol::table tilemaps = i_scene["tilemap"];

	int i = 0;

	while (true) {
		sol::optional<sol::table> hasTilemap = tilemaps[i];
		if (hasTilemap == sol::nullopt)
			break;
		sol::table tilemap = tilemaps[i];

		std::string mapPath = tilemap["map_path"];
		std::string mapAssetId = tilemap["texture_asset_id"];
		int mapRows = tilemap["num_rows"];
		int mapCols = tilemap["num_cols"];
		int tileSize = tilemap["tile_size"];
		double tileScale = tilemap["tile_scale"];
		bool isWalkable = tilemap["walkable"];

		std::fstream stream;
		stream.open(mapPath);

		std::string str;
		int x, y;
		for (y = 0; std::getline(stream, str); y++) {
			std::istringstream ss(str);
			std::string token;

			for (x = 0; std::getline(ss, token, ','); x++) {
				int tileId = std::stoi(token) - 1;
				if (tileId == -1) continue;

				Entity tile = i_entityManager->CreateEntity();
				tile.AddComponent<TransformComponent>(glm::vec2(tileSize * x * tileScale, tileSize * y * tileScale),
					glm::vec2(tileScale, tileScale), 0.0);
				if (!isWalkable)
					tile.AddComponent<BoxColliderComponent>(tileSize, tileSize);

				int startingX = tileSize * ((tileId % mapCols));
				int startingY = tileSize * (tileId / mapCols);

				tile.SetGroup(Group::TILES_G);
				tile.AddComponent<SpriteComponent>(mapAssetId, tileSize, tileSize, RenderLayer::BACKGROUND_L, SDL_RendererFlip::SDL_FLIP_NONE,
					startingX, startingY);
			}
		}
		stream.close();

		Game::MapWidth = x * tileSize * tileScale;
		Game::MapHeight = y * tileSize * tileScale;

		i++;
	}
}