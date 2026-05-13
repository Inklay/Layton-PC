#include "Game/Layton1/Scene/Puzzle/Puzzle.h"
#include "Game/Game.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"

namespace Layton1Scene {
	Puzzle::Puzzle(Game* game, int picarat, int number, const std::string& internalName, Character character) :
		Scene(game, 0.5f, 0.5f),
		m_picarat(picarat),
		m_currentPicarat(picarat),
		m_number(number),
		m_internalName(internalName),
		m_character(character)
	{
	}

	void Puzzle::load() {
		// intro
		m_sprites.insert({ "intro_title", std::make_unique<Sprite>("bg/q_alt_sub_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_picaratBackground", std::make_unique<Sprite>("bg/fr/picarat_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_puzzleText", std::make_unique<Sprite>("ani/fr/pazzle_mes.png", this, SDL_FRect{ centerXPos(88), HALF_HEIGHT + 30, 88, 14})});
		m_sprites.insert({ "intro_bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }, true) });
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

		// hints
		m_sprites.insert({ "hint0", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb4.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}, true) });
		m_sprites.insert({ "hint1", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb3.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}, true) });
		m_sprites.insert({ "hint2", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb2.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}, true) });
		m_sprites.insert({ "hint3", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb1.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}, true) });
		m_sprites.insert({ "bottomBackgroundHint1", std::make_unique<Sprite>("bg/hint_1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint2", std::make_unique<Sprite>("bg/hint_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint3", std::make_unique<Sprite>("bg/hint_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint1Locked", std::make_unique<Sprite>("bg/fr/hint_1_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint2Locked", std::make_unique<Sprite>("bg/fr/hint_2_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint3Locked", std::make_unique<Sprite>("bg/fr/hint_3_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint1NoCoin", std::make_unique<Sprite>("bg/fr/hint_1_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint2NoCoin", std::make_unique<Sprite>("bg/fr/hint_2_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackgroundHint3NoCoin", std::make_unique<Sprite>("bg/fr/hint_3_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "hintBackButton", std::make_unique<Sprite>("ani/fr/buttons.12.png", this, SDL_FRect{ WIDTH - 56, HALF_HEIGHT, 56, 20 }, true) });
		m_sprites.insert({ "hint1Button", std::make_unique<Sprite>("ani/fr/buttons.5.png", this, SDL_FRect{ 0, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hint2Button", std::make_unique<Sprite>("ani/fr/buttons.6.png", this, SDL_FRect{ 56, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hint3Button", std::make_unique<Sprite>("ani/fr/buttons.7.png", this, SDL_FRect{ 112, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hint1LockedButton", std::make_unique<Sprite>("ani/fr/buttons.8.png", this, SDL_FRect{ 0, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hint2LockedButton", std::make_unique<Sprite>("ani/fr/buttons.9.png", this, SDL_FRect{ 56, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hint3LockedButton", std::make_unique<Sprite>("ani/fr/buttons.10.png", this, SDL_FRect{ 112, HALF_HEIGHT, 56, 16 }, true) });
		m_sprites.insert({ "hintUnlockYesButton", std::make_unique<ClickableSprite>("ani/fr/yesnobuttons.0.png", "ani/fr/yesnobuttons.1.png", this, SDL_FRect{ 53, HALF_HEIGHT + 140, 64, 24 }) });
		m_sprites.insert({ "hintUnlockNoButton", std::make_unique<ClickableSprite>("ani/fr/yesnobuttons.2.png", "ani/fr/yesnobuttons.3.png", this, SDL_FRect{ 137, HALF_HEIGHT + 140, 64, 24 }) });
		m_sprites.insert({ "hint1Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_1.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{0, 0, 0}) });
		m_sprites.insert({ "hint2Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_2.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{0, 0, 0}) });
		m_sprites.insert({ "hint3Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_3.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{0, 0, 0}) });

		// validation
		if (m_character == LUKE) {
			m_sprites.insert({ "validate1", std::make_unique<Sprite>("bg/judge_r1_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate2", std::make_unique<Sprite>("bg/judge_r2_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate3", std::make_unique<Sprite>("bg/judge_r3_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate4", std::make_unique<Sprite>("bg/judge_r4_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate5", std::make_unique<Sprite>("bg/judge_r5_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate6", std::make_unique<Sprite>("bg/judge_r6_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			
			m_sprites.insert({ "validateSuccess1", std::make_unique<Sprite>("bg/judge_r7_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess2", std::make_unique<Sprite>("bg/judge_r8_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess3", std::make_unique<Sprite>("bg/judge_r9_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess4", std::make_unique<Sprite>("bg/judge_r10_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess5", std::make_unique<Sprite>("bg/judge_r11.bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess6", std::make_unique<Sprite>("bg/judge_r12_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess7", std::make_unique<Sprite>("bg/judge_r13_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });

			m_sprites.insert({ "validateFail1", std::make_unique<Sprite>("bg/judge_r107_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail2", std::make_unique<Sprite>("bg/judge_r108_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail3", std::make_unique<Sprite>("bg/judge_r109_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail4", std::make_unique<Sprite>("bg/judge_r110_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail5", std::make_unique<Sprite>("bg/judge_r111.bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail6", std::make_unique<Sprite>("bg/judge_r112_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail7", std::make_unique<Sprite>("bg/judge_r113_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
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

			if (m_fadingToPuzzle && !m_sprites.at("fading")->m_fading) {
				m_fadingToPuzzle = false;
				m_isIntro = false;
				m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
				playBGM("sound/SEQ_BG_003.wav");
			}
		} else if (m_validating) {
			if (m_fadingToValidation && !m_sprites.at("fading")->m_fading) {
				m_fadingToValidation = false;
				m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
				playSFX("puzzleEnd");
			} else if (!m_fadingToValidation) {
				if (m_validationTimer >= 2350) {
					if (validate()) {
						m_sprites.at("validateSuccess7")->draw();
					} else {
						m_sprites.at("validateFail7")->draw();
					}
				} else if (m_validationTimer >= 2250) {
					if (validate()) {
						m_sprites.at("validateSuccess6")->draw();
					} else {
						m_sprites.at("validateFail6")->draw();
					}
				} else if (m_validationTimer >= 2150) {
					if (validate()) {
						m_sprites.at("validateSuccess5")->draw();
					} else {
						m_sprites.at("validateFail5")->draw();
					}
				} else if (m_validationTimer >= 2100) {
					if (validate()) {
						m_sprites.at("validateSuccess4")->draw();
					} else {
						m_sprites.at("validateFail4")->draw();
					}
				} else if (m_validationTimer >= 1600) {
					if (validate()) {
						m_sprites.at("validateSuccess3")->draw();
					} else {
						m_sprites.at("validateFail3")->draw();
					}
				} else if (m_validationTimer >= 1500) {
					if (validate()) {
						m_sprites.at("validateSuccess2")->draw();
					} else {
						m_sprites.at("validateFail2")->draw();
					}
				} else if (m_validationTimer >= 1400) {
					if (validate()) {
						m_sprites.at("validateSuccess1")->draw();
					} else {
						m_sprites.at("validateFail1")->draw();
					}
				} else if (m_validationTimer >= 900) {
					m_sprites.at("validate6")->draw();
				} else if (m_validationTimer >= 800) {
					m_sprites.at("validate5")->draw();
				} else if (m_validationTimer >= 700) {
					m_sprites.at("validate4")->draw();
				} else if (m_validationTimer >= 200) {
					m_sprites.at("validate3")->draw();
				} else if (m_validationTimer >= 100) {
					m_sprites.at("validate2")->draw();
				} else {
					m_sprites.at("validate1")->draw();
				}
				
				m_validationTimer += (int)(SDL_GetTicks() - m_lastTick);
			}
		} else {
			m_sprites.at("topBackground")->draw();
			m_sprites.at("puzzleNumber0")->draw();
			m_sprites.at("puzzleNumber1")->draw();
			m_sprites.at("puzzleNumber2")->draw();
			m_sprites.at("picarat0")->draw();
			m_sprites.at("picarat1")->draw();
			m_sprites.at("puzzleText")->draw();
			m_sprites.at("hintCoins0")->draw();

			if (m_game->m_save->m_hintCoins > 9) {
				m_sprites.at("hintCoins1")->draw();
			}

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
				m_sprites.at("hint3")->draw(1);
			} else if ((m_game->m_save->m_puzzles.at(m_number) & 4) == 0 && !m_bottomUIHidden) {
				m_sprites.at("hint2")->draw(1);
			} else if ((m_game->m_save->m_puzzles.at(m_number) & 8) == 0 && !m_bottomUIHidden) {
				m_sprites.at("hint1")->draw(1);
			} else if (!m_bottomUIHidden) {
				m_sprites.at("hint0")->draw(1);
			}

			if (m_displayHint) {
				if (m_currentHint == 0) {
					if (m_game->m_save->m_puzzles.at(m_number) & 2) {
						m_sprites.at("bottomBackgroundHint1")->draw(10);
						m_sprites.at("hint1Text")->draw(10);
					} else if ((m_game->m_save->m_hintCoins != 0)) {
						m_sprites.at("bottomBackgroundHint1Locked")->draw(10);
						m_sprites.at("hintUnlockYesButton")->draw(11);
						m_sprites.at("hintUnlockNoButton")->draw(11);
						displayHintCoinsOnHintUnlockScreen();
					} else {
						m_sprites.at("bottomBackgroundHint1NoCoin")->draw(10);
					}
				} else if (m_currentHint == 1) {
					if (m_game->m_save->m_puzzles.at(m_number) & 4) {
						m_sprites.at("bottomBackgroundHint2")->draw(10);
						m_sprites.at("hint2Text")->draw(10);
					} else if ((m_game->m_save->m_hintCoins != 0)) {
						m_sprites.at("bottomBackgroundHint2Locked")->draw(10);
						m_sprites.at("hintUnlockYesButton")->draw(11);
						m_sprites.at("hintUnlockNoButton")->draw(11);
						displayHintCoinsOnHintUnlockScreen();
					} else {
						m_sprites.at("bottomBackgroundHint2NoCoin")->draw(10);
					}
				} else if (m_currentHint == 2) {
					if (m_game->m_save->m_puzzles.at(m_number) & 8) {
						m_sprites.at("bottomBackgroundHint3")->draw(10);
						m_sprites.at("hint3Text")->draw(10);
					} else if ((m_game->m_save->m_hintCoins != 0)) {
						m_sprites.at("bottomBackgroundHint3Locked")->draw(10);
						m_sprites.at("hintUnlockYesButton")->draw(11);
						m_sprites.at("hintUnlockNoButton")->draw(11);
						displayHintCoinsOnHintUnlockScreen();
					} else {
						m_sprites.at("bottomBackgroundHint3NoCoin")->draw(10);
					}
				}

				if (m_game->m_save->m_puzzles.at(m_number) & 2) {
					m_sprites.at("hint1Button")->draw(11);

					if (m_game->m_save->m_puzzles.at(m_number) & 4) {
						m_sprites.at("hint2Button")->draw(11);

						if (m_game->m_save->m_puzzles.at(m_number) & 8) {
							m_sprites.at("hint3Button")->draw(11);
						} else {
							m_sprites.at("hint3LockedButton")->draw(11);
						}
					} else {
						m_sprites.at("hint2LockedButton")->draw(11);
					}
				} else {
					m_sprites.at("hint1LockedButton")->draw(11);
				}

				m_sprites.at("hintBackButton")->draw(11);
			}
		}

		if (m_sprites.at("fading")->m_fading) {
			m_sprites.at("fading")->draw();
		}

		Scene::render();
	}

	void Puzzle::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_isIntro && !m_fadingToPuzzle && m_movedTitleCard && !m_sprites.at("fading")->m_fading && !m_sprites.at("intro_title")->m_translating && !m_validating) {
			m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::IN });
			m_fadingToPuzzle = true;
		} else if (!m_isIntro && m_textProgression <= m_text.length() && !m_validating) {
			m_textProgression = (int)m_text.length();
			m_game->m_bgmData.at(1)->loop = false;
		} else if (!m_isIntro && !m_validating) {
			if (spriteName == "hint0" || spriteName == "hint1" || spriteName == "hint2" || spriteName == "hint3") {
				m_sprites.at("bottomBackgroundHint1")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint2")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint3")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint1Locked")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint2Locked")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint3Locked")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint1NoCoin")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint2NoCoin")->m_interactive = true;
				m_sprites.at("bottomBackgroundHint3NoCoin")->m_interactive = true;
				m_sprites.at("hintBackButton")->m_interactive = true;
				m_sprites.at("hint1Button")->m_interactive = true;
				m_sprites.at("hint2Button")->m_interactive = true;
				m_sprites.at("hint3Button")->m_interactive = true;
				m_sprites.at("hint1LockedButton")->m_interactive = true;
				m_sprites.at("hint2LockedButton")->m_interactive = true;
				m_sprites.at("hint3LockedButton")->m_interactive = true;
				m_sprites.at("hintUnlockYesButton")->m_interactive = true;
				m_sprites.at("hintUnlockNoButton")->m_interactive = true;
				m_displayHint = true;
			} else if ((spriteName == "hintBackButton" || spriteName == "hintUnlockNoButton") && m_displayHint) {
				m_sprites.at("bottomBackgroundHint1")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint2")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint3")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint1Locked")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint2Locked")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint3Locked")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint1NoCoin")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint2NoCoin")->m_interactive = false;
				m_sprites.at("bottomBackgroundHint3NoCoin")->m_interactive = false;
				m_sprites.at("hintBackButton")->m_interactive = false;
				m_sprites.at("hint1Button")->m_interactive = false;
				m_sprites.at("hint2Button")->m_interactive = false;
				m_sprites.at("hint3Button")->m_interactive = false;
				m_sprites.at("hint1LockedButton")->m_interactive = false;
				m_sprites.at("hint2LockedButton")->m_interactive = false;
				m_sprites.at("hint3LockedButton")->m_interactive = false;
				m_sprites.at("hintUnlockYesButton")->m_interactive = false;
				m_sprites.at("hintUnlockNoButton")->m_interactive = false;
				m_displayHint = false;
			} else if (spriteName == "hintUnlockYesButton" && m_displayHint) {
				for (size_t i = 0; i < 3; i++) {
					if (m_sprites.count("hintCoins" + std::to_string(i)) != 0) {
						m_sprites.at("hintCoins" + std::to_string(i))->unload();
						m_sprites.at("hintCoins" + std::to_string(i)).release();
						m_sprites.erase("hintCoins" + std::to_string(i));
					}
				}

				m_game->m_save->m_hintCoins--;
				std::vector<fileUtils::path> numberSprites = getNumberSprites(m_game->m_save->m_hintCoins, "q_numbers");

				for (size_t i = 0; i < numberSprites.size(); i++) {
					m_sprites.insert({ "hintCoins" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 241.0f - i * 8, 6, 8, 10}) });
				}

				if (m_currentHint == 0) {
					m_game->m_save->m_puzzles.at(m_number) += 2;
				} else if (m_currentHint == 1) {
					m_game->m_save->m_puzzles.at(m_number) += 4;
				} else if (m_currentHint == 2) {
					m_game->m_save->m_puzzles.at(m_number) += 8;
				}
			} else if (spriteName == "hint1Button") {
				m_currentHint = 0;
			} else if (spriteName == "hint2Button" || spriteName == "hint2LockedButton") {
				m_currentHint = 1;
			} else if (spriteName == "hint3Button" || spriteName == "hint3LockedButton") {
				m_currentHint = 2;
			}
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

	void Puzzle::displayHintCoinsOnHintUnlockScreen() {
		float x = m_sprites.at("hintCoins0")->m_transform.x;
		float y = m_sprites.at("hintCoins0")->m_transform.y;

		m_sprites.at("hintCoins0")->m_transform.x = 190 * m_game->m_windowMultiplier;
		m_sprites.at("hintCoins0")->m_transform.y = (HALF_HEIGHT + 107) * m_game->m_windowMultiplier;

		m_sprites.at("hintCoins0")->draw();

		m_sprites.at("hintCoins0")->m_transform.x = x;
		m_sprites.at("hintCoins0")->m_transform.y = y;


		if (m_game->m_save->m_hintCoins > 9) {
			x = m_sprites.at("hintCoins1")->m_transform.x;
			y = m_sprites.at("hintCoins1")->m_transform.y;

			m_sprites.at("hintCoins1")->m_transform.x = 182 * m_game->m_windowMultiplier;
			m_sprites.at("hintCoins1")->m_transform.y = (HALF_HEIGHT + 107) * m_game->m_windowMultiplier;

			m_sprites.at("hintCoins1")->draw();

			m_sprites.at("hintCoins1")->m_transform.x = x;
			m_sprites.at("hintCoins1")->m_transform.y = y;
		}
	}

	void Puzzle::startValidation() {
		m_validating = true;
		m_fadingToValidation = true;
		m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::IN });
	}
};
