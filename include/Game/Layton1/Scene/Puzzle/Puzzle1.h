#pragma once

#include "Game/Layton1/Scene/Puzzle/Puzzle.h"

namespace Layton1Scene {

	class Puzzle1 : public Puzzle {
	public:
		Puzzle1(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);
	};
};
