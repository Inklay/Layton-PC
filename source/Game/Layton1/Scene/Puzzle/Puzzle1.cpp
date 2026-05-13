#include "Game/Layton1/Scene/Puzzle/Puzzle1.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Puzzle1::Puzzle1(Game* game, const std::string& nextScene) :
		Puzzle(game, 10, 1, "24", LUKE, 2, nextScene)
	{
	}

	void Puzzle1::load() {
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/fr/q24_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "tuto", std::make_unique<Sprite>("bg/fr/puzzlet22_0.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "redrawCircle", std::make_unique<Sprite>("ani/fr/retry_trace.png", this, SDL_FRect{ centerXPos(180), centerBottomPos(17), 180, 17})});
		m_sprites.insert({ "pointer", std::make_unique<Sprite>("ani/point_trace.png", this, SDL_FRect{ 0, 0, 24, 24}) });

		m_canClear = true;
		m_canValidate = true;
		m_drawTexture = SDL_CreateTexture(m_game->m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HALF_HEIGHT);
		if (!m_drawTexture) {
			std::cout << SDL_GetError() << std::endl;
		}
		m_drawTextureRect = SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT };
		sdlUtils::multiply(m_drawTextureRect, m_game->m_windowMultiplier);
		SDL_SetTextureScaleMode(m_drawTexture, SDL_SCALEMODE_NEAREST);

		m_villages = {
			{ 46, 58 },
			{ 100, 76 },
			{ 145, 88 },
			{ 122, 136 },
			{ 56, 141 }
		};

		Puzzle::load();
	}

	void Puzzle1::render() {
		if (m_state >= PUZZLE_FADING_IN && m_state <= PUZZLE_FADING_OUT) {
			if (!m_sprites.at("tuto")->m_fading) {
				m_sprites.at("bottomBackground")->draw();
			}

			if (m_tutoState == 2 && !m_sprites.at("tuto")->m_fading) {
				m_tutoState++;
				showBottomUI();
			}

			if (m_tutoState == 1 || m_tutoState == 2) {
				m_sprites.at("tuto")->draw();
			}

			SDL_SetRenderTarget(m_game->m_renderer, m_drawTexture);

			for (int i = 1; i < m_points.size(); i++) {
				renderLine(m_game->m_renderer, m_points.at(i - 1).x, m_points.at(i - 1).y, m_points.at(i).x, m_points.at(i).y, 3, { 1.0f, 0.0f, 0.0f, 1.0f });
				renderFilledCircle(m_game->m_renderer, m_points.at(i - 1).x, m_points.at(i - 1).y, 1, { 1.0f, 0.0f, 0.0f, 1.0f });
			}

			SDL_SetRenderTarget(m_game->m_renderer, NULL);
			SDL_RenderTexture(m_game->m_renderer, m_drawTexture, NULL, &m_drawTextureRect);

			if (m_points.size() > 0 && !m_drawing) {
				SDL_FPoint center = getShapeCenter(m_points);
				m_targetedVillaged = -1;

				for (int i = 0; i < m_villages.size(); i++) {
					if (center.x > m_villages.at(i).x - 20 && center.x < m_villages.at(i).x + 20 && center.y > m_villages.at(i).y - 20 && center.y < m_villages.at(i).y + 20) {
						m_targetedVillaged = i;
						break;
					}
				}

				if (isClosedShape(m_points) && m_targetedVillaged != -1) {
					m_sprites.at("pointer")->m_transform.y = (m_villages.at(m_targetedVillaged).y - 24 + HALF_HEIGHT) * m_game->m_windowMultiplier;
					m_sprites.at("pointer")->m_transform.x = m_villages.at(m_targetedVillaged).x * m_game->m_windowMultiplier;
					m_sprites.at("pointer")->draw();
				} else {
					m_sprites.at("redrawCircle")->draw();
				}
			}
		}

		Puzzle::render();
	}

	void Puzzle1::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_state >= PUZZLE_TOUCH && m_state <= PUZZLE) {
			if (m_tutoState == 0) {
				m_tutoState++;
				hideBottomUI();
			} else if (m_tutoState == 1) {
				m_sprites.at("tuto")->fade({ 500, 0, Sprite::FadingMode::OUT });
				m_tutoState++;
				playSFX("keyboardSwitch");
			}

			if (spriteName == "clearButton") {
				m_points.clear();
				SDL_SetRenderTarget(m_game->m_renderer, m_drawTexture);
				SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, 0);
				SDL_RenderClear(m_game->m_renderer);
				SDL_SetRenderTarget(m_game->m_renderer, NULL);
				playSFX("clear");
			} else if (spriteName == "validateButton" && m_targetedVillaged != -1) {
				m_points.clear();
				SDL_SetRenderTarget(m_game->m_renderer, m_drawTexture);
				SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, 0);
				SDL_RenderClear(m_game->m_renderer);
				SDL_SetRenderTarget(m_game->m_renderer, NULL);
				playSFX("keyboardOk");
				startValidation();
			}
		}

		Puzzle::handleClick(spriteName, event);
	}

	void Puzzle1::handleEvent(SDL_Event event) {
		if (m_state == PUZZLE && m_tutoState == 3) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				m_drawing = true;
				m_points.clear();
				SDL_SetRenderTarget(m_game->m_renderer, m_drawTexture);
				SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, 0);
				SDL_RenderClear(m_game->m_renderer);
				SDL_SetRenderTarget(m_game->m_renderer, NULL);
			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
				m_drawing = false;
			} else if (event.type == SDL_EVENT_MOUSE_MOTION && m_drawing) {
				m_points.emplace_back(SDL_FPoint{ event.motion.x / m_game->m_windowMultiplier, (event.motion.y / m_game->m_windowMultiplier) - HALF_HEIGHT });
			}
		}

		Scene::handleEvent(event);
	}

	void Puzzle1::unload() {
		SDL_DestroyTexture(m_drawTexture);
		m_points.clear();
		m_villages.clear();

		Puzzle::unload();
	}

	bool Puzzle1::validate() {
		return m_targetedVillaged == 0;
	}
};
