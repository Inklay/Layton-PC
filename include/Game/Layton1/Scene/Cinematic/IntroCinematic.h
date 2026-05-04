#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class IntroCinematic : public Scene {
	public:
		IntroCinematic(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event) {};
	};
};
