#include "Game/Layton1/Scene/Puzzle/Puzzle.h"
#include "Game/Game.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include <cmath>

namespace Layton1Scene {
	Puzzle::Puzzle(Game* game, int picarat, int number, const std::string& internalName, Character character, int picaratLost, const std::string& nextScene) :
		Scene(game, 0.5f, 0.5f),
		m_internalName(internalName),
		m_character(character),
		m_nextSceneName(nextScene),
		m_picarat(picarat),
		m_currentPicarat(picarat),
		m_newPicarat(picarat),
		m_number(number),
		m_picaratLost(picaratLost)
	{
	}

	void Puzzle::load() {
		m_game->m_save->m_puzzles.at(m_number) += 16;

		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }, true) });
		m_sprites.insert({ "touch", std::make_unique<Sprite>("ani/fr/qend_touch.png", this, SDL_FRect{ centerXPos(128), centerBottomPos(64), 128, 64}) });

		m_sprites.at("touch")->m_opacity = 180;

		// intro
		m_sprites.insert({ "introTitle", std::make_unique<Sprite>("bg/q_alt_sub_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "introPicaratBackground", std::make_unique<Sprite>("bg/fr/picarat_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "introPuzzleText", std::make_unique<Sprite>("ani/fr/pazzle_mes.png", this, SDL_FRect{ centerXPos(88), HALF_HEIGHT + 30, 88, 14}) });
		m_sprites.insert({ "introPuzzleName", std::make_unique<TextSprite>("font/fontevent.png", "qtext/fr/t_" + m_internalName + ".txt", this, SDL_FRect{-1, HALF_HEIGHT + 120, WIDTH, 12}, SDL_Color{ 0, 0, 0, 255 }) });

		std::vector<fileUtils::path> numberSprites = getNumberSprites(m_currentPicarat, "picarat_number_big");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "introCurrentPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 92.0f - i * 40, HALF_HEIGHT + 55, 36, 58}) });
		}

		numberSprites = getNumberSprites(m_picarat, "picarat_get_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "introPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 183.0f - i * 28, HALF_HEIGHT + 77, 22, 36}) });
		}

		numberSprites = getNumberSprites(m_game->m_save->m_picarats, "picarat_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "introTotalPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 188.0f - i * 16, HEIGHT - 22, 8, 14}) });
		}

		numberSprites = getNumberSprites(m_number, "qintro_numbers", 3);
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "introPuzzleNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 175.0f - i * 32, HALF_HEIGHT + 54, 32, 48}) });
		}

		// puzzle
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/q_bg.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "puzzleText", std::make_unique<TextSprite>("font/fontq.png", std::u32string(U""), this, SDL_FRect{5, 23, WIDTH - 10, HALF_HEIGHT - 30}, SDL_Color{ 0, 0, 0, 255 }) });

		numberSprites = getNumberSprites(m_number, "q_numbers", 3);
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "puzzleNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 42.0f - i * 8, 6, 8, 10}) });
		}

		numberSprites = getNumberSprites(m_picarat, "q_numbers");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "picarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 87.0f - i * 8, 6, 8, 10}) });
		}

		numberSprites = getNumberSprites(m_game->m_save->m_currentHintCoins, "q_numbers");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "hintCoins" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 241.0f - i * 8, 6, 8, 10}) });
		}

		if (m_canQuit) {
			m_sprites.insert({ "quitButton", std::make_unique<Sprite>("ani/fr/pass_button.png", this, SDL_FRect{ WIDTH - 72, HALF_HEIGHT + 24, 72, 18}, true) });
			m_sprites.insert({ "quitButtonYes", std::make_unique<Sprite>("ani/fr/yesnobuttons.1.png", this, SDL_FRect{ 53, HALF_HEIGHT + 120, 64, 24 }) });
			m_sprites.insert({ "quitButtonNo", std::make_unique<Sprite>("ani/fr/yesnobuttons.3.png", this, SDL_FRect{ 137, HALF_HEIGHT + 120, 64, 24 }) });
			m_sprites.insert({ "quitBackground", std::make_unique<Sprite>("bg/later_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT}) });
			m_sprites.insert({ "quitText", std::make_unique<Sprite>("ani/fr/later_mes.png", this, SDL_FRect{ centerXPos(208), HALF_HEIGHT + 78, 208, 16})});
			m_sprites.insert({ "quitName", std::make_unique<TextSprite>("font/fontevent.png", "qtext/fr/t_" + m_internalName + ".txt", this, SDL_FRect{-1, HALF_HEIGHT + 55, WIDTH, 12}, SDL_Color{ 0, 0, 0, 255 }) });
		}

		if (m_canClear) {
			m_sprites.insert({ "clearButton", std::make_unique<Sprite>("ani/fr/clear_btn.png", this, SDL_FRect{ WIDTH - 64, HALF_HEIGHT + 45, 64, 18}, true) });
		}

		if (m_canValidate) {
			m_sprites.insert({ "validateButton", std::make_unique<Sprite>("ani/fr/submit.png", this, SDL_FRect{ WIDTH - 70, HEIGHT - 36, 66, 26}, true) });
		}

		if (m_sprites.count("bottomBackground") == 0) {
			m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/q" + m_internalName + "_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		}

		// hints
		m_sprites.insert({ "hint0", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb4.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint1", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb3.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint2", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb2.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "hint3", std::make_unique<AnimatedSprite>("ani/fr/hint_buttons.hintb1.anim", this, SDL_FRect{ WIDTH - 72 ,HALF_HEIGHT, 72, 18}) });
		m_sprites.insert({ "bottomBackgroundHint1", std::make_unique<Sprite>("bg/hint_1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint2", std::make_unique<Sprite>("bg/hint_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint3", std::make_unique<Sprite>("bg/hint_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint1Locked", std::make_unique<Sprite>("bg/fr/hint_1_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint2Locked", std::make_unique<Sprite>("bg/fr/hint_2_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint3Locked", std::make_unique<Sprite>("bg/fr/hint_3_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint1NoCoin", std::make_unique<Sprite>("bg/fr/hint_1_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint2NoCoin", std::make_unique<Sprite>("bg/fr/hint_2_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundHint3NoCoin", std::make_unique<Sprite>("bg/fr/hint_3_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "hintBackButton", std::make_unique<Sprite>("ani/fr/buttons.12.png", this, SDL_FRect{ WIDTH - 56, HALF_HEIGHT, 56, 20 }) });
		m_sprites.insert({ "hint1Button", std::make_unique<Sprite>("ani/fr/buttons.5.png", this, SDL_FRect{ 0, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hint2Button", std::make_unique<Sprite>("ani/fr/buttons.6.png", this, SDL_FRect{ 56, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hint3Button", std::make_unique<Sprite>("ani/fr/buttons.7.png", this, SDL_FRect{ 112, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hint1LockedButton", std::make_unique<Sprite>("ani/fr/buttons.8.png", this, SDL_FRect{ 0, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hint2LockedButton", std::make_unique<Sprite>("ani/fr/buttons.9.png", this, SDL_FRect{ 56, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hint3LockedButton", std::make_unique<Sprite>("ani/fr/buttons.10.png", this, SDL_FRect{ 112, HALF_HEIGHT, 56, 16 }) });
		m_sprites.insert({ "hintUnlockYesButton", std::make_unique<ClickableSprite>("ani/fr/yesnobuttons.0.png", "ani/fr/yesnobuttons.1.png", this, SDL_FRect{ 53, HALF_HEIGHT + 140, 64, 24 }) });
		m_sprites.insert({ "hintUnlockNoButton", std::make_unique<ClickableSprite>("ani/fr/yesnobuttons.2.png", "ani/fr/yesnobuttons.3.png", this, SDL_FRect{ 137, HALF_HEIGHT + 140, 64, 24 }) });
		m_sprites.insert({ "hint1Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_1.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{ 0, 0, 0, 255 }) });
		m_sprites.insert({ "hint2Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_2.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{ 0, 0, 0, 255 }) });
		m_sprites.insert({ "hint3Text", std::make_unique<TextSprite>("font/fontq.png", "qtext/fr/h_" + m_internalName + "_3.txt", this, SDL_FRect{ 4, HALF_HEIGHT + 20, WIDTH - 8, HALF_HEIGHT - 23 }, SDL_Color{ 0, 0, 0, 255 }) });

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
			m_sprites.insert({ "validateFail5", std::make_unique<Sprite>("bg/judge_r111_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail6", std::make_unique<Sprite>("bg/judge_r112_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail7", std::make_unique<Sprite>("bg/judge_r113_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		} else {
			m_sprites.insert({ "validate1", std::make_unique<Sprite>("bg/judge_l1_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate2", std::make_unique<Sprite>("bg/judge_l2_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate3", std::make_unique<Sprite>("bg/judge_l3_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate4", std::make_unique<Sprite>("bg/judge_l4_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate5", std::make_unique<Sprite>("bg/judge_l5_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validate6", std::make_unique<Sprite>("bg/judge_l6_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });

			m_sprites.insert({ "validateSuccess1", std::make_unique<Sprite>("bg/judge_l7_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess2", std::make_unique<Sprite>("bg/judge_l8_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess3", std::make_unique<Sprite>("bg/judge_l9_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess4", std::make_unique<Sprite>("bg/judge_l10_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess5", std::make_unique<Sprite>("bg/judge_l11.bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess6", std::make_unique<Sprite>("bg/judge_l12_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateSuccess7", std::make_unique<Sprite>("bg/judge_l13_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });

			m_sprites.insert({ "validateFail1", std::make_unique<Sprite>("bg/judge_l107_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail2", std::make_unique<Sprite>("bg/judge_l108_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail3", std::make_unique<Sprite>("bg/judge_l109_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail4", std::make_unique<Sprite>("bg/judge_l110_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail5", std::make_unique<Sprite>("bg/judge_l111_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail6", std::make_unique<Sprite>("bg/judge_l112_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
			m_sprites.insert({ "validateFail7", std::make_unique<Sprite>("bg/judge_l113_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		}

		// end
		m_sprites.insert({ "endText", std::make_unique<TextSprite>("font/fontq.png", std::u32string(U""), this, SDL_FRect{5, HALF_HEIGHT + 21, WIDTH - 10, HALF_HEIGHT - 30}, SDL_Color{ 0, 0, 0, 255 }) });

		m_sprites.insert({ "failTitle", std::make_unique<Sprite>("bg/fr/judge_l114_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "failTextBackground", std::make_unique<Sprite>("bg/qend_awrong.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });

		m_sprites.insert({ "successTitle", std::make_unique<Sprite>("bg/fr/judge_l14_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "successTextBackground", std::make_unique<Sprite>("bg/qend_acorrect.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "successPicaratBackground", std::make_unique<Sprite>("bg/fr/picarat_get_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "successTopBackground", std::make_unique<Sprite>("bg/q" + m_internalName + "a_bg.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });

		if (m_canQuit) {
			m_sprites.insert({ "endQuitBackground", std::make_unique<Sprite>("bg/qend_retry.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
			m_sprites.insert({ "endRetryButton", std::make_unique<ClickableSprite>("ani/fr/ato_btn.0.png", this, SDL_FRect{ centerXPos(152), 27 + HALF_HEIGHT, 152, 36}) });
			m_sprites.insert({ "endHintButton", std::make_unique<ClickableSprite>("ani/fr/ato_btn.1.png", this, SDL_FRect{ centerXPos(152), 78 + HALF_HEIGHT, 152, 36 }) });
			m_sprites.insert({ "endQuitButton", std::make_unique<ClickableSprite>("ani/fr/ato_btn.2.png", this, SDL_FRect{ centerXPos(152), 129 + HALF_HEIGHT, 152, 36 }) });
		}

		m_text = fileUtils::readText(m_game->m_gameFolder / "qtext/fr" / ("q_" + m_internalName + ".txt"));
		m_failText = fileUtils::readText(m_game->m_gameFolder / "qtext/fr" / ("f_" + m_internalName + ".txt"));
		m_successText = fileUtils::readText(m_game->m_gameFolder / "qtext/fr" / ("c_" + m_internalName + ".txt"));
		m_fading = true;
		playSFX("puzzleStart");
		Scene::load();
	}

	void Puzzle::render() {
		if (m_state <= INTRO_FADING_OUT) {
			renderIntro();
		} else if (m_state >= PUZZLE_FADING_IN && m_state <= HINT) {
			renderPuzzle();
		} else if (m_state >= VALIDATING_FADING_IN && m_state <= VALIDATING_FADING_OUT) {
			renderValidation();
		} else if (m_state >= END_FADING_IN) {
			if (m_valid) {
				renderSuccess();
			} else {
				renderFail();
			}
		}

		if (m_sprites.at("fading")->m_fading) {
			m_sprites.at("fading")->draw(-1);
		}

		if (m_sprites.at("bottomFading")->m_fading) {
			m_sprites.at("bottomFading")->draw(-1);
		}

		Scene::render();
	}

	void Puzzle::handleClick(const std::string& spriteName, SDL_Event) {
		if (m_state == INTRO_FULL) {
			m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::IN });
			m_state = INTRO_FADING_OUT;
		} else if (m_state == PUZZLE_TOUCH) {
			m_textProgression = (int)m_text.length();
			m_game->m_bgmData.at(1)->loop = false;
			m_state = PUZZLE;

			playSFX("puzzleClicked");
		} else if (m_state == PUZZLE) {
			if (spriteName == "hint0" || spriteName == "hint1" || spriteName == "hint2" || spriteName == "hint3") {
				enableHintButtons();
				m_state = HINT;

				playSFX("showHints");
			} else if (spriteName == "quitButton") {
				m_sprites.at("quitButtonYes")->m_interactive = true;
				m_sprites.at("quitButtonNo")->m_interactive = true;
				m_state = PUZZLE_QUIT;

				playSFX("keyboardSwitch");
			}
		} else if (m_state == PUZZLE_QUIT) {
			if (spriteName == "quitButtonNo") {
				m_sprites.at("quitButtonYes")->m_interactive = false;
				m_sprites.at("quitButtonNo")->m_interactive = false;
				m_state = PUZZLE;

				playSFX("hideHints");
			} else if (spriteName == "quitButtonYes") {
				fadeToNextScene(m_nextSceneName);
				playSFX("keyboardSwitch");
			}
		} else if (m_state == HINT || m_state == END_HINT) {
			if ((spriteName == "hintBackButton" || spriteName == "hintUnlockNoButton")) {
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

				if (m_state == PUZZLE) {
					m_state = PUZZLE;
				} else {
					m_state = END_FADING_OUT;
					m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::IN });
					fadeBGM(400);
				}

				playSFX("hideHints");
			} else if (spriteName == "hintUnlockYesButton") {
				m_game->m_save->m_currentHintCoins--;
				std::vector<fileUtils::path> numberSprites = getNumberSprites(m_game->m_save->m_currentHintCoins, "q_numbers");
				updateNumberSprite(numberSprites, "hintCoins", SDL_FRect{ 241.0, 6, 8, 10 }, 8);

				if (m_currentHint == 0) {
					m_game->m_save->m_puzzles.at(m_number) += 2;
				} else if (m_currentHint == 1) {
					m_game->m_save->m_puzzles.at(m_number) += 4;
				} else if (m_currentHint == 2) {
					m_game->m_save->m_puzzles.at(m_number) += 8;
				}

				playSFX("unlockHint");
			} else if (spriteName == "hint1Button" && m_currentHint != 0) {
				m_currentHint = 0;
				playSFX("switchHint");
			} else if ((spriteName == "hint2Button" || spriteName == "hint2LockedButton") && m_currentHint != 1) {
				m_currentHint = 1;
				playSFX("switchHint");
			} else if ((spriteName == "hint3Button" || spriteName == "hint3LockedButton") && m_currentHint != 2) {
				m_currentHint = 2;
				playSFX("switchHint");
			}
		} else if (m_state == END_FULL) {
			if (m_valid) {
				playSFX("keyboardSwitch");
				fadeToNextScene(m_nextSceneName);
			} else {
				if (m_canQuit) {
					m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::IN });
					m_state = END_TEXT_FADING_OUT;
					m_game->m_bgmData.at(1)->loop = false;
					playSFX("keyboardSwitch");
				} else {
					m_state = END_FADING_OUT;
					m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::IN });
					fadeBGM(400);
					playSFX("keyboardSwitch");
				}
			}
		} else if (m_state == END_PICARAT) {
			if (!(m_game->m_save->m_puzzles.at(m_number) & 1)) {
				m_game->m_save->m_puzzles.at(m_number) += 1;
				m_game->m_save->m_solvedPuzzles++;
			}
			m_state = END_PICARAT_FADING_OUT;
			m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::IN });
		} else if (m_state == END_QUIT) {
			m_sprites.at("endRetryButton")->m_interactive = false;
			m_sprites.at("endHintButton")->m_interactive = false;
			m_sprites.at("endQuitButton")->m_interactive = false;

			if (spriteName == "endRetryButton") {
				m_state = END_FADING_OUT;
				fadeBGM(400);
				playSFX("keyboardSwitch");
			} else if (spriteName == "endQuitButton") {
				playSFX("keyboardSwitch");
				fadeToNextScene(m_nextSceneName);
			} else if (spriteName == "endHintButton") {
				m_state = END_QUIT_FADING_OUT;
				m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::IN });
				playSFX("keyboardSwitch");
			}
		}
	}

	void Puzzle::enableHintButtons() {
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

		for (size_t i = 1; i < points.size(); i++) {
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
		m_failText.clear();
		m_successText.clear();

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


		if (m_game->m_save->m_currentHintCoins > 9) {
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
		m_state = PUZZLE_FADING_OUT;
		m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::IN });
		m_valid = validate();
		fadeBGM(0.3f, 0);
		
		int voiceLine = 1 + (rand() % 3);
		
		if (m_character == LUKE) {
			playSFX("lukeAnswer" + std::to_string(voiceLine), 1);
		} else {
			playSFX("laytonAnswer" + std::to_string(voiceLine), 1);
		}
	}

	void Puzzle::renderIntro() {
		if (!m_fading && m_state == INTRO && !m_sprites.at("fading")->m_fading) {
			m_sprites.at("introTitle")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_sprites.at("introPuzzleText")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_sprites.at("introPuzzleName")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_sprites.at("introPuzzleNumber0")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_sprites.at("introPuzzleNumber1")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_sprites.at("introPuzzleNumber2")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
			m_state = INTRO_CARD_MOVING;
		} else if (m_state == INTRO_CARD_MOVING && !m_sprites.at("introTitle")->m_translating) {
			m_state = INTRO_PICARAT_FADING;
			m_sprites.at("bottomFading")->fade({ 300, -200, Sprite::FadingMode::OUT });
		} else if (m_state == INTRO_PICARAT_FADING && !m_sprites.at("bottomFading")->m_fading) {
			m_state = m_newPicarat == m_currentPicarat ? INTRO_FULL : INTRO_LOOSING_PICARAT;
		} else if (m_state == INTRO_LOOSING_PICARAT) {
			if (m_currentPicarat == m_newPicarat) {
				m_state = INTRO_FULL;
				std::vector<fileUtils::path> numberSprites = getNumberSprites(m_currentPicarat, "q_numbers");
				updateNumberSprite(numberSprites, "picarat", SDL_FRect{ 87, 6, 8, 10 }, 8);
			} else {
				m_picaratChangeTimer++;

				if (m_picaratChangeTimer == 3) {
					m_currentPicarat--;
					m_picaratChangeTimer = 0;

					std::vector<fileUtils::path> numberSprites = getNumberSprites(m_currentPicarat, "picarat_number_big");
					updateNumberSprite(numberSprites, "introCurrentPicarat", SDL_FRect{ 92, HALF_HEIGHT + 55, 36, 58 }, 40);
					playSFX("puzzlePicaratDecrease");
				}
			}
		} else if (m_state == INTRO_FADING_OUT && !m_sprites.at("fading")->m_fading) {
			m_state = PUZZLE_FADING_IN;
			m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			playBGM("sound/SEQ_BG_003.wav");
		}

		if (m_state >= INTRO_PICARAT_FADING) {
			m_sprites.at("introPicaratBackground")->draw();
			m_sprites.at("introPicarat0")->draw();
			m_sprites.at("introPicarat1")->draw();
			m_sprites.at("introTotalPicarat0")->draw();

			m_sprites.at("introCurrentPicarat0")->draw();

			if (m_currentPicarat >= 10) {
				m_sprites.at("introCurrentPicarat1")->draw();
			}

			if (m_game->m_save->m_picarats >= 10) {
				m_sprites.at("introTotalPicarat1")->draw();
			}

			if (m_game->m_save->m_picarats >= 100) {
				m_sprites.at("introTotalPicarat2")->draw();
			}

			if (m_game->m_save->m_picarats >= 1000) {
				m_sprites.at("introTotalPicarat3")->draw();
			}
		}

		m_sprites.at("introTitle")->draw();
		m_sprites.at("introPuzzleText")->draw();
		m_sprites.at("introPuzzleName")->draw();
		m_sprites.at("introPuzzleNumber0")->draw();
		m_sprites.at("introPuzzleNumber1")->draw();
		m_sprites.at("introPuzzleNumber2")->draw();
	}

	void Puzzle::renderPuzzle() {
		if (m_state == PUZZLE_FADING_IN && !m_sprites.at("fading")->m_fading) {
			m_state = PUZZLE_TOUCH;
		}
			
		if (m_state >= PUZZLE_FADING_IN && m_state <= PUZZLE_FADING_OUT) {
			if (m_canQuit && !m_bottomUIHidden) {
				m_sprites.at("quitButton")->draw(1);
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

			if (m_state == PUZZLE_TOUCH) {
				m_sprites.at("touch")->draw();
				computeTouchTextOpacity();
			} else if (m_state == PUZZLE_FADING_OUT && !m_sprites.at("fading")->m_fading) {
				m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
				m_state = VALIDATING_FADING_IN;
			}
		} else if (m_state == HINT) {
			renderHint();
		} else if (m_state == PUZZLE_QUIT) {
			m_sprites.at("quitBackground")->draw();
			m_sprites.at("quitButtonYes")->draw(1);
			m_sprites.at("quitButtonNo")->draw(1);
			m_sprites.at("quitName")->draw();
			m_sprites.at("quitText")->draw();
		}

		m_sprites.at("topBackground")->draw();
		m_sprites.at("puzzleNumber0")->draw();
		m_sprites.at("puzzleNumber1")->draw();
		m_sprites.at("puzzleNumber2")->draw();
		m_sprites.at("puzzleText")->draw();
		m_sprites.at("hintCoins0")->draw();

		if (m_game->m_save->m_currentHintCoins > 9) {
			m_sprites.at("hintCoins1")->draw();
		}

		m_sprites.at("picarat0")->draw();

		if (m_currentPicarat >= 10) {
			m_sprites.at("picarat1")->draw();
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
	}

	void Puzzle::renderHint() {
		if (m_currentHint == 0) {
			if (m_game->m_save->m_puzzles.at(m_number) & 2) {
				m_sprites.at("bottomBackgroundHint1")->draw(10);
				m_sprites.at("hint1Text")->draw(10);
			} else if ((m_game->m_save->m_currentHintCoins != 0)) {
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
			} else if ((m_game->m_save->m_currentHintCoins != 0)) {
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
			} else if ((m_game->m_save->m_currentHintCoins != 0)) {
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

	void Puzzle::renderValidation() {
		if (m_state == VALIDATING_FADING_IN && !m_sprites.at("fading")->m_fading) {
			m_state = VALIDATING;
			playSFX("puzzleEnd");
		} else {
			if (m_state == VALIDATING_FADING_OUT && !m_sprites.at("fading")->m_fading) {
				m_sprites.at("fading")->fade({ 300,0, Sprite::FadingMode::OUT });
				m_state = END_FADING_IN;
			} else if (m_validationTimer >= 2350) {
				if (m_state == VALIDATING_SOUND) {
					m_state = VALIDATING_FADING_OUT;
					m_sprites.at("fading")->fade({ 300,-300, Sprite::FadingMode::IN });
				}

				if (m_valid) {
					m_sprites.at("validateSuccess7")->draw();
				} else {
					m_sprites.at("validateFail7")->draw();
				}
			} else if (m_validationTimer >= 2250) {
				if (m_valid) {
					m_sprites.at("validateSuccess6")->draw();
				} else {
					m_sprites.at("validateFail6")->draw();
				}
			} else if (m_validationTimer >= 2150) {
				if (m_valid) {
					m_sprites.at("validateSuccess5")->draw();
				} else {
					m_sprites.at("validateFail5")->draw();
				}
			} else if (m_validationTimer >= 2100) {
				if (m_state == VALIDATING) {
					m_state = VALIDATING_SOUND;
					if (m_valid) {
						playSFX("puzzleSuccess");
					} else {
						playSFX("puzzleFailure");
					}
				}
				if (m_valid) {
					m_sprites.at("validateSuccess4")->draw();
				} else {
					m_sprites.at("validateFail4")->draw();
				}
			} else if (m_validationTimer >= 1600) {
				if (m_valid) {
					m_sprites.at("validateSuccess3")->draw();
				} else {
					m_sprites.at("validateFail3")->draw();
				}
			} else if (m_validationTimer >= 1500) {
				if (m_valid) {
					m_sprites.at("validateSuccess2")->draw();
				} else {
					m_sprites.at("validateFail2")->draw();
				}
			} else if (m_validationTimer >= 1400) {
				if (m_valid) {
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
	}

	void Puzzle::renderSuccess() {
		if (m_state >= END_PICARAT_FADING_IN && m_state < END_TEXT_FADING_IN) {
			m_sprites.at("successPicaratBackground")->draw();

			if (m_sprites.count("endTotalPicarat0")) {
				m_sprites.at("endTotalPicarat0")->draw();
			}

			if (m_sprites.count("endTotalPicarat1")) {
				m_sprites.at("endTotalPicarat1")->draw();
			}

			if (m_sprites.count("endTotalPicarat2")) {
				m_sprites.at("endTotalPicarat2")->draw();
			}

			if (m_sprites.count("endTotalPicarat3")) {
				m_sprites.at("endTotalPicarat3")->draw();
			}

			if (m_sprites.count("endCurrentPicarat0")) {
				m_sprites.at("endCurrentPicarat0")->draw();
			}

			if (m_sprites.count("endCurrentPicarat1")) {
				m_sprites.at("endCurrentPicarat1")->draw();
			}

			if (m_sprites.count("endCurrentPicarat2")) {
				m_sprites.at("endCurrentPicarat2")->draw();
			}
		} else if (m_state >= END_TEXT_FADING_IN) {
			m_sprites.at("successTextBackground")->draw();
			m_sprites.at("successTopBackground")->draw();
			m_sprites.at("puzzleNumber0")->draw();
			m_sprites.at("puzzleNumber1")->draw();
			m_sprites.at("puzzleNumber2")->draw();
			m_sprites.at("hintCoins0")->draw();

			if (m_game->m_save->m_currentHintCoins > 9) {
				m_sprites.at("hintCoins1")->draw();
			}
		}

		if (m_state < END_TEXT_FADING_IN) {
			m_sprites.at("successTitle")->draw();
		}

		if (m_state == END_FADING_IN && !m_sprites.at("fading")->m_fading) {
			m_state = END_CARD_MOVING;
			m_sprites.at("successTitle")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
		} else if (m_state == END_CARD_MOVING && !m_sprites.at("successTitle")->m_translating) {
			m_state = END_PICARAT_FADING_IN;
			m_sprites.at("bottomFading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			int voiceLine = 1 + (rand() % 3);

			if (m_character == LUKE) {
				playSFX("lukeRight" + std::to_string(voiceLine), 1);
			} else {
				playSFX("laytonRight" + std::to_string(voiceLine), 1);
			}
		} else if (m_state == END_PICARAT_FADING_IN && !m_sprites.at("bottomFading")->m_fading) {
			m_state = END_PICARAT_ADDING;
			m_picaratChangeTimer = 0;
		} else if (m_state == END_PICARAT_ADDING) {
			if (m_currentPicarat == 0) {
				m_state = END_PICARAT;
			} else {
				m_picaratChangeTimer++;

				if (m_picaratChangeTimer == 3) {
					m_currentPicarat--;
					m_game->m_save->m_picarats++;
					m_picaratChangeTimer = 0;

					std::vector<fileUtils::path> numberSprites = getNumberSprites(m_game->m_save->m_picarats, "picarat_get_number_big");
					updateNumberSprite(numberSprites, "endTotalPicarat", SDL_FRect{ 165, HEIGHT - 65, 26, 44 }, 30);

					numberSprites = getNumberSprites(m_currentPicarat, "picarat_get_number_small");
					updateNumberSprite(numberSprites, "endCurrentPicarat", SDL_FRect{ 130, HALF_HEIGHT + 40, 22, 36 }, 26);
					playSFX("puzzlePicaratDecrease", 2);
				}
			}
		} else if (m_state == END_PICARAT_FADING_OUT && !m_sprites.at("fading")->m_fading) {
			m_state = END_TEXT_FADING_IN;
			m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			m_sprites.at("puzzleNumber0")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("puzzleNumber1")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("puzzleNumber2")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("hintCoins0")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("endText")->m_transform.y += 2 * m_game->m_windowMultiplier;

			if (m_game->m_save->m_currentHintCoins >= 10) {
				m_sprites.at("hintCoins1")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			}
		} else if (m_state == END_TEXT_FADING_IN && !m_sprites.at("bottomFading")->m_fading) {
			m_state = END_FULL;
			m_textProgression = 0;
			m_game->m_bgmData.at(1)->loop = true;
			m_game->m_bgmData.at(0)->position = 0;
			m_game->m_bgmData.at(0)->fading = false;
			m_game->m_bgmData.at(0)->volume = 1.0f;
		} else if (m_state == END_FULL) {
			m_sprites.at("endText")->draw();
			m_sprites.at("touch")->draw();
			computeTouchTextOpacity();

			if (m_textProgression <= m_successText.length()) {
				if (m_textProgression == 0) {
					playBGM("sound/sfx/99.wav", 1);
				}

				m_sprites.at("endText")->setText(m_successText.substr(0, m_textProgression));
				m_textProgression += 2;

				if (m_textProgression > m_successText.length()) {
					m_textProgression--;
					m_game->m_bgmData.at(1)->loop = false;
				}
			}
		}
	}

	void Puzzle::renderFail() {
		if (m_state == END_FADING_OUT && !m_sprites.at("fading")->m_fading) {
			playSFX("puzzleStart");
			pauseBGM(0);

			if ((m_game->m_save->m_puzzles.at(m_number) & 16) == 0) {
				m_game->m_save->m_puzzles.at(m_number) += 16;
				m_newPicarat -= m_picaratLost;
			} else if ((m_game->m_save->m_puzzles.at(m_number) & 32) == 0) {
				m_game->m_save->m_puzzles.at(m_number) += 32;
				m_newPicarat -= m_picaratLost;
			}

			m_state = INTRO;
			m_game->m_bgmData.at(0)->position = 0;
			m_game->m_bgmData.at(0)->fading = false;
			m_game->m_bgmData.at(0)->volume = 1.0f;
			m_validationTimer = 0;

			m_sprites.at("fading")->fade({ 300, -300, Sprite::FadingMode::OUT });
			m_sprites.at("introTitle")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("introPuzzleText")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("introPuzzleName")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("introPuzzleNumber0")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("introPuzzleNumber1")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("introPuzzleNumber2")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			m_sprites.at("failTitle")->m_transform.y += HALF_HEIGHT * m_game->m_windowMultiplier;
			return;
		} else if (m_state >= END_TEXT_FADING_IN) {
			m_sprites.at("failTextBackground")->draw();
		}

		if (m_state == END_FADING_IN && !m_sprites.at("fading")->m_fading) {
			m_state = END_CARD_MOVING;
			m_sprites.at("failTitle")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
		} else if (m_state == END_CARD_MOVING && !m_sprites.at("failTitle")->m_translating) {
			m_state = END_TEXT_FADING_IN;
			m_sprites.at("bottomFading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			int voiceLine = 1 + (rand() % 3);

			if (m_character == LUKE) {
				playSFX("lukeWrong" + std::to_string(voiceLine), 1);
			} else {
				playSFX("laytonWrong" + std::to_string(voiceLine), 1);
			}
		} else if (m_state == END_TEXT_FADING_IN && !m_sprites.at("bottomFading")->m_fading) {
			m_state = END_FULL;
			m_textProgression = 0;
			m_game->m_bgmData.at(1)->loop = true;
			m_game->m_bgmData.at(0)->position = 0;
			m_game->m_bgmData.at(0)->fading = false;
			m_game->m_bgmData.at(0)->volume = 1.0f;
		} else if (m_state == END_TEXT_FADING_OUT && !m_sprites.at("bottomFading")->m_fading) {
			m_sprites.at("endRetryButton")->m_interactive = true;
			m_sprites.at("endHintButton")->m_interactive = true;
			m_sprites.at("endQuitButton")->m_interactive = true;
			m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::OUT });
			m_state = END_QUIT_FADING_IN;
		} else if (m_state >= END_QUIT_FADING_IN && m_state <= END_QUIT_FADING_OUT) {
			m_sprites.at("endQuitBackground")->draw();
			m_sprites.at("endRetryButton")->draw(1);
			m_sprites.at("endHintButton")->draw(1);
			m_sprites.at("endQuitButton")->draw(1);

			if (m_state == END_QUIT_FADING_IN && !m_sprites.at("bottomFading")->m_fading) {
				m_state = END_QUIT;
			} else if (m_state == END_QUIT_FADING_OUT && !m_sprites.at("bottomFading")->m_fading) {
				m_state = END_HINT_FADING_IN;
				m_sprites.at("fading")->fade({ 400, 0, Sprite::FadingMode::OUT });
			}
		} else if (m_state >= END_HINT_FADING_IN && m_state <= END_HINT) {
			if (m_state == END_HINT_FADING_IN && !m_sprites.at("bottomFading")->m_fading) {
				m_state = END_HINT;
				enableHintButtons();
			}

			renderHint();
		} else if (m_state == END_FULL) {
			m_sprites.at("endText")->draw();
			m_sprites.at("touch")->draw();
			computeTouchTextOpacity();

			if (m_textProgression <= m_failText.length()) {
				if (m_textProgression == 0) {
					playBGM("sound/sfx/99.wav", 1);
				}

				m_sprites.at("endText")->setText(m_failText.substr(0, m_textProgression));
				m_textProgression += 2;

				if (m_textProgression > m_failText.length()) {
					m_textProgression--;
					m_game->m_bgmData.at(1)->loop = false;
				}
			}
		}

		m_sprites.at("failTitle")->draw();
	}

	void Puzzle::computeTouchTextOpacity() {
		m_touchTextTimer += (int)(SDL_GetTicks() - m_lastTick);

		if (m_touchTextState == 0 && m_touchTextTimer >= 1000) {
			m_touchTextState = 1;
			m_touchTextTimer = 0;
		} else if (m_touchTextState == 1) {
			int opacity = 180 - ((m_touchTextTimer * 110) / 350);

			if (opacity <= 70) {
				opacity = 70;
				m_touchTextState = 2;
				m_touchTextTimer = 0;
			}

			m_sprites.at("touch")->m_opacity = opacity;
		} else if (m_touchTextState == 2 && m_touchTextTimer >= 300) {
			m_touchTextState = 3;
			m_touchTextTimer = 0;
		} else if (m_touchTextState == 3) {
			int opacity = 70 + ((m_touchTextTimer * 110) / 350);

			if (opacity >= 180) {
				opacity = 180;
				m_touchTextState = 0;
				m_touchTextTimer = 0;
			}

			m_sprites.at("touch")->m_opacity = opacity;
		}
	}
	
	void Puzzle::updateNumberSprite(const std::vector<fileUtils::path>& sprites, const std::string& name, SDL_FRect baseRect, float spacing) {
		for (size_t i = 0; i < 4; i++) {
			if (m_sprites.count(name + std::to_string(i)) != 0) {
				m_sprites.at(name + std::to_string(i))->unload();
				m_sprites.at(name + std::to_string(i)).release();
				m_sprites.erase(name + std::to_string(i));
			} else {
				break;
			}
		}

		for (size_t i = 0; i < sprites.size(); i++) {
			baseRect.x -= i * spacing;
			m_sprites.insert({ name + std::to_string(i), std::make_unique<Sprite>(sprites.at(i), this, baseRect) });
		}
	}

	bool Puzzle::validate() {
		return m_valid;
	}
};
