#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {
	class Map : public Scene {
	public:
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	protected:
		Map(Game* game);
		void findHintCoin();

		std::vector<std::vector<bool>> m_hintCoins;
	};
};
