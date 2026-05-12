#include "Game/Layton1/Scene/Puzzle/Puzzle1.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Puzzle1::Puzzle1(Game* game) :
		Puzzle(game, 10, 1, "qtext/fr/t_24.txt")
	{
	}

	void Puzzle1::load() {
		Puzzle::load();
	}

	void Puzzle1::render() {
		Puzzle::render();
	}

	void Puzzle1::handleClick(const std::string& spriteName, SDL_Event event) {
		Puzzle::handleClick(spriteName, event);
	}
};
