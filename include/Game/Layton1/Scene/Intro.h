#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class Intro : public Scene {
	public:
		Intro(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);
	};

};
