#include "Game/Layton1/Scene/Puzzle/Puzzle2.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Puzzle2::Puzzle2(Game* game, const std::string& nextScene) :
		Puzzle(game, 15, 2, "23", LAYTON, 5, nextScene)
	{}

	void Puzzle2::load() {
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/fr/q23_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "gear1", std::make_unique<Sprite>("ani/q23_gear.0.png", this, SDL_FRect{ 0, HALF_HEIGHT, 102, 96}, true) });
		m_sprites.insert({ "gear2", std::make_unique<Sprite>("ani/q23_gear.1.png", this, SDL_FRect{ 0, HALF_HEIGHT, 102, 96}, true) });
		m_sprites.insert({ "gear3", std::make_unique<Sprite>("ani/q23_gear.2.png", this, SDL_FRect{ 0, HALF_HEIGHT, 102, 96}, true) });
		m_sprites.insert({ "gear4", std::make_unique<Sprite>("ani/q23_gear.3.png", this, SDL_FRect{ 0, HALF_HEIGHT, 102, 96}, true) });
		m_sprites.insert({ "button", std::make_unique<Sprite>("ani/q23_button.png", this, SDL_FRect{ 0, 0, 16, 16}, true) });

		m_sprites.at("button")->m_opacity = 0;
		m_canQuit = true;
		m_gears = {
			{ 39, 138 + HALF_HEIGHT, 16, 16 },
			{ 121, 116 + HALF_HEIGHT, 16, 16 },
			{ 202, 126 + HALF_HEIGHT, 16, 16 }
		};

		Puzzle::load();
	}

	void Puzzle2::render() {
		if (m_state >= PUZZLE_FADING_IN && m_state <= PUZZLE_FADING_OUT) {
			m_sprites.at("bottomBackground")->draw();
			m_sprites.at("button")->draw();

			switch (m_angleShown) {
				case 0:
					m_sprites.at("gear1")->draw(1);
					break;
				case 1:
					m_sprites.at("gear2")->draw();
					break;
				case 2:
					m_sprites.at("gear3")->draw();
					break;
				case 3:
					m_sprites.at("gear4")->draw();
					break;
			}
		}

		Puzzle::render();
	}

	void Puzzle2::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_state == PUZZLE) {
			if (spriteName == "gear1") {
				playSFX("switchHint");
				m_angleShown++;

				if (m_angleShown > 3) {
					m_angleShown = 0;
				}
			}
		}

		Puzzle::handleClick(spriteName, event);
	}

	void Puzzle2::handleEvent(SDL_Event event) {
		if (m_state == PUZZLE) {
			SDL_FPoint coord = { event.button.x / m_game->m_windowMultiplier, event.button.y / m_game->m_windowMultiplier };

			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				for (int i = 0; i < m_gears.size(); i++) {
					if (coord.x >= m_gears.at(i).x && coord.x < m_gears.at(i).x + m_gears.at(i).w && coord.y >= m_gears.at(i).y && coord.y < m_gears.at(i).y + m_gears.at(i).h) {
						SDL_FRect rect = m_gears.at(i);
						sdlUtils::multiply(rect, m_game->m_windowMultiplier);
						m_sprites.at("button")->m_transform = rect;
						m_sprites.at("button")->m_opacity = 255;
					}
				}
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				m_sprites.at("button")->m_opacity = 0;

				for (int i = 0; i < m_gears.size(); i++) {
					if (coord.x >= m_gears.at(i).x && coord.x < m_gears.at(i).x + m_gears.at(i).w && coord.y >= m_gears.at(i).y && coord.y < m_gears.at(i).y + m_gears.at(i).h) {
						m_valid = i == 0;
						startValidation();
					}
				}
			}
		}

		Puzzle::handleEvent(event);
	}
};
