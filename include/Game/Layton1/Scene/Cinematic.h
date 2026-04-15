#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class Cinematic : public Scene {
	public:
		Cinematic(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event) {};
	};
};
