#include "Game/Layton1/Scene/Puzzle/Puzzle1.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Puzzle1::Puzzle1(Game* game) :
		Puzzle(game, 10, 1, "24")
	{
	}

	void Puzzle1::load() {
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/fr/q24_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_canClear = true;
		m_canValidate = true;

		Puzzle::load();
	}

	void Puzzle1::render() {
		if (!m_isIntro) {
			m_sprites.at("bottomBackground")->draw();
		}

		Puzzle::render();
	}

	void Puzzle1::handleClick(const std::string& spriteName, SDL_Event event) {
		Puzzle::handleClick(spriteName, event);
	}
};
