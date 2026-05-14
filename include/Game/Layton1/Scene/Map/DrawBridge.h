#pragma once

#include "Game/Layton1/Scene/Map/Map.h"

namespace Layton1Scene {
	class DrawBridge : public Map {
	public:
		DrawBridge(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);
	};
};
