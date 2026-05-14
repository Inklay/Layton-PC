#include "Game/Layton1/Scene/Map/Map.h"

namespace Layton1Scene {
	Map::Map(Game* game) : 
		Scene(game)
	{
	}

	void Map::load() {
		Scene::load();
	}

	void Map::render() {
		Scene::render();
	}

	void Map::handleClick(const std::string& spriteName, SDL_Event event) {

	}

	void Map::findHintCoin() {

	}
};
