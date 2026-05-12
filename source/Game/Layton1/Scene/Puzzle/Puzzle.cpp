#include "Game/Layton1/Scene/Puzzle/Puzzle.h"
#include "Game/Game.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Sprite/AnimatedSprite.h"

namespace Layton1Scene {
	Puzzle::Puzzle(Game* game, int picarat, int number, const std::string& internalName) :
		Scene(game, 0.5f, 0.5f),
		m_picarat(picarat),
		m_currentPicarat(picarat),
		m_number(number),
		m_internalName(internalName)
	{
	}

	void Puzzle::load() {
		// intro
		m_sprites.insert({ "intro_title", std::make_unique<Sprite>("bg/q_alt_sub_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_picaratBackground", std::make_unique<Sprite>("bg/fr/picarat_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_puzzleText", std::make_unique<Sprite>("ani/fr/pazzle_mes.png", this, SDL_FRect{ centerXPos(88), HALF_HEIGHT + 30, 88, 14})});
		m_sprites.insert({ "intro_bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "intro_fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }, true) });
		m_sprites.insert({ "intro_puzzleName", std::make_unique<TextSprite>("font/fontevent.png", "qtext/fr/t_" + m_internalName + ".txt", this, SDL_FRect{-1, HALF_HEIGHT + 120, WIDTH, 12}, SDL_Color{0, 0, 0})});

		std::vector<fileUtils::path> numberSprites = getNumberSprites(m_currentPicarat, "picarat_number_big");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_currentPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 92.0f - i * 40, HALF_HEIGHT + 55, 36, 58})});
		}

		numberSprites = getNumberSprites(m_picarat, "picarat_get_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_picarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 183.0f - i * 28, HALF_HEIGHT + 77, 22, 36}) });
		}

		numberSprites = getNumberSprites(m_game->m_save->m_picarats, "picarat_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_totalPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 188.0f - i * 16, HEIGHT - 22, 8, 14}) });
		}

		numberSprites = getNumberSprites(m_number, "qintro_numbers", 3);
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_puzzleNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 175.0f - i * 32, HALF_HEIGHT + 54, 32, 48}) });
		}

		// puzzle
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/q_bg.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "puzzleText", std::make_unique<TextSprite>("font/fontq.png", std::u32string(U""), this, SDL_FRect{5, 23, WIDTH - 10, HALF_HEIGHT - 30}, SDL_Color{0, 0, 0})});
		m_sprites.insert({ "hint0", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb4.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint1", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb3.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint2", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb2.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint3", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb1.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });


		numberSprites = getNumberSprites(m_number, "q_numbers", 3);
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "puzzleNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 42.0f - i * 8, 6, 8, 10}) });
		}

		numberSprites = getNumberSprites(m_picarat, "q_numbers");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "picarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 87.0f - i * 8, 6, 8, 10}) });
		}

		numberSprites = getNumberSprites(m_game->m_save->m_hintCoins, "q_numbers");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "hintCoins" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 241.0f - i * 8, 6, 8, 10}) });
		}

		if (m_canClear) {
			m_sprites.insert({ "clearButton", std::make_unique<Sprite>("ani/fr/clear_btn.png", this, SDL_FRect{ WIDTH - 64, HALF_HEIGHT + 45, 64, 18}, true) });
		}

		if (m_canValidate) {
			m_sprites.insert({ "validateButton", std::make_unique<Sprite>("ani/fr/submit.png", this, SDL_FRect{ WIDTH - 70, HEIGHT - 36, 66, 26}, true) });
		}

		m_text = fileUtils::readText(m_game->m_gameFolder / "qtext/fr" / ("q_" + m_internalName + ".txt"));
		m_fading = true;
		playSFX("puzzleStart");
		m_sprites.at("intro_bottomFading")->fade({ 300, -1000, Sprite::FadingMode::OUT });
		Scene::load();
	}

	void Puzzle::render() {
		if (m_isIntro) {
			if (!m_fading && !m_movedTitleCard) {
				m_sprites.at("intro_title")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleText")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleName")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber0")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber1")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber2")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_movedTitleCard = true;
			}

			if (m_movedTitleCard && !m_sprites.at("intro_title")->m_translating) {
				m_sprites.at("intro_picaratBackground")->draw();

				m_sprites.at("intro_currentPicarat0")->draw();
				m_sprites.at("intro_currentPicarat1")->draw();

				m_sprites.at("intro_picarat0")->draw();
				m_sprites.at("intro_picarat1")->draw();

				m_sprites.at("intro_totalPicarat0")->draw();
				if (m_game->m_save->m_picarats > 10) {
					m_sprites.at("intro_totalPicarat2")->draw();
				}
				if (m_game->m_save->m_picarats > 100) {
					m_sprites.at("intro_totalPicarat3")->draw();
				}
				if (m_game->m_save->m_picarats > 1000) {
					m_sprites.at("intro_totalPicarat4")->draw();
				}
			}
			m_sprites.at("intro_bottomFading")->draw();
			m_sprites.at("intro_title")->draw();
			m_sprites.at("intro_puzzleText")->draw();
			m_sprites.at("intro_puzzleName")->draw();
			m_sprites.at("intro_puzzleNumber0")->draw();
			m_sprites.at("intro_puzzleNumber1")->draw();
			m_sprites.at("intro_puzzleNumber2")->draw();

			if (m_fadingToPuzzle && !m_sprites.at("intro_fading")->m_fading) {
				m_fadingToPuzzle = false;
				m_isIntro = false;
				m_sprites.at("intro_fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
				playBGM("sound/SEQ_BG_003.wav");
			}
		} else {
			m_sprites.at("topBackground")->draw();
			m_sprites.at("puzzleNumber0")->draw();
			m_sprites.at("puzzleNumber1")->draw();
			m_sprites.at("puzzleNumber2")->draw();
			m_sprites.at("picarat0")->draw();
			m_sprites.at("picarat1")->draw();
			m_sprites.at("hintCoins0")->draw();
			m_sprites.at("hintCoins1")->draw();
			m_sprites.at("puzzleText")->draw();

			if (m_textProgression <= m_text.length()) {
				if (m_textProgression == 0) {
					playBGM("sound/sfx/99.wav", 1);
				}

				m_sprites.at("puzzleText")->setText(m_text.substr(0, m_textProgression));
				m_textProgression += 2;

				if (m_textProgression > m_text.length()) {
					m_textProgression--;
					m_game->m_bgmData.at(1)->loop = false;
				}
			}

			if (m_canClear && !m_bottomUIHidden) {
				m_sprites.at("clearButton")->draw(1);
			}

			if (m_canValidate && !m_bottomUIHidden) {
				m_sprites.at("validateButton")->draw(1);
			}

			if ((m_game->m_save->m_puzzles.at(m_number) & 2) == 0 && !m_bottomUIHidden) {
				m_sprites.at("hint3")->draw();
			} else if ((m_game->m_save->m_puzzles.at(m_number) & 4) == 0 && !m_bottomUIHidden) {
				m_sprites.at("hint2")->draw();
			} else if ((m_game->m_save->m_puzzles.at(m_number) & 8) == 0 && !m_bottomUIHidden) {
				m_sprites.at("hint1")->draw();
			} else if (!m_bottomUIHidden) {
				m_sprites.at("hint0")->draw();
			}
		}

		if (m_sprites.at("intro_fading")->m_fading) {
			m_sprites.at("intro_fading")->draw();
		}

		Scene::render();
	}

	void Puzzle::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_isIntro && !m_fadingToPuzzle && m_movedTitleCard && !m_sprites.at("intro_fading")->m_fading && !m_sprites.at("intro_title")->m_translating) {
			m_sprites.at("intro_fading")->fade({ 300, 0, Sprite::FadingMode::IN });
			m_fadingToPuzzle = true;
		} else if (!m_isIntro && m_textProgression <= m_text.length()) {
			m_textProgression = (int)m_text.length();
			m_game->m_bgmData.at(1)->loop = false;
		}
	}

	std::vector<fileUtils::path> Puzzle::getNumberSprites(int number, const std::string& font, int padding) {
		std::vector<fileUtils::path> vec;
		int digit;

		if (number == 0) {
			vec.emplace_back("ani/" + font + ".0.png");
			return vec;
		}

		while (number != 0) {
			digit = number % 10;
			vec.emplace_back("ani/" + font + "." + std::to_string(digit) + ".png");
			number /= 10;
		}

		while (vec.size() < padding) {
			vec.emplace_back("ani/" + font + ".0.png");
		}

		return vec;
	}

	void Puzzle::showBottomUI() {
		m_bottomUIHidden = false;
	}

	void Puzzle::hideBottomUI() {
		m_bottomUIHidden = true;
	}

	bool Puzzle::isClosedShape(std::vector<SDL_FPoint> points) {
		if (points.size() < 10) {
			return false;
		}

		float dx = points.front().x - points.back().x;
		float dy = points.front().y - points.back().y;
		float distEndToStart = sqrtf(dx * dx + dy * dy);
		float totalLength = 0;

		for (int i = 1; i < points.size(); i++) {
			float ldx = points.at(i).x - points.at(i - 1).x;
			float ldy = points.at(i).y - points.at(i - 1).y;
			totalLength += sqrtf(ldx * ldx + ldy * ldy);
		}

		float closingRatio = distEndToStart / totalLength;
		return closingRatio < 0.15f;
	}

	SDL_FPoint Puzzle::getShapeCenter(std::vector<SDL_FPoint> points) {
		float sumX = 0, sumY = 0;

		for (auto& p : points) {
			sumX += p.x;
			sumY += p.y;
		}
		return { sumX / points.size(), sumY / points.size() };
	}

	void Puzzle::unload() {
		m_internalName.clear();
		m_text.clear();

		Scene::unload();
	}
};
