#pragma once

#include "Game/Layton1/Scene/Puzzle/Puzzle.h"

namespace Layton1Scene {

	class Puzzle2 : public Puzzle {
	public:
		Puzzle2(Game* game, const std::string& nextScene);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);
		void handleEvent(SDL_Event event);

	private:
		int m_angleShown = 0;
		std::vector<SDL_FRect> m_gears;
	};
};
