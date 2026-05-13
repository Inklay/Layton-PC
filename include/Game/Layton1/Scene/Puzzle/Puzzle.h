#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class Puzzle : public Scene {
	public:
		enum Character {
			LAYTON,
			LUKE
		};

		enum State {
			INTRO,
			INTRO_CARD_MOVING,
			INTRO_PICARAT_FADING,
			INTRO_LOOSING_PICARAT,
			INTRO_FULL,
			INTRO_FADING_OUT,
			PUZZLE_FADING_IN,
			PUZZLE_TOUCH,
			PUZZLE,
			PUZZLE_FADING_OUT,
			HINT,
			VALIDATING_FADING_IN,
			VALIDATING,
			VALIDATING_SOUND,
			VALIDATING_FADING_OUT,
			END_FADING_IN,
			END_CARD_MOVING,
			END_TEXT_FADING,
			END_FULL,
			END_FADING_OUT
		};

		Puzzle(Game* game, int picarat, int number, const std::string& internalName, Character character, int picaratLost);
		void load();
		void render();
		void unload();
		void handleClick(const std::string& spriteName, SDL_Event event);
		void startValidation();

	protected:
		std::vector<fileUtils::path> getNumberSprites(int number, const std::string& font, int padding = 0);
		void updateNumberSprite(const std::vector<fileUtils::path>& sprites, const std::string& name, SDL_FRect baseRect, float spacing);
		void hideBottomUI();
		void showBottomUI();
		virtual bool validate() = 0;
		bool isClosedShape(std::vector<SDL_FPoint> points);
		SDL_FPoint getShapeCenter(std::vector<SDL_FPoint> points);

		std::string m_internalName;
		std::u32string m_text;
		std::u32string m_failText;
		std::u32string m_successText;
		Character m_character;
		State m_state = INTRO;
		int m_picarat;
		int m_currentPicarat;
		int m_number;
		int m_picaratChangeTimer = 0;
		int m_textProgression = 0;
		int m_currentHint = 0;
		int m_validationTimer = 0;
		int m_touchTextState = 0;
		int m_touchTextTimer = 0;
		int m_picaratLost;
		int m_newPicarat;
		bool m_canClear = false;
		bool m_canValidate = false;
		bool m_bottomUIHidden = false;
		bool m_valid = false;

	private:
		void displayHintCoinsOnHintUnlockScreen();
		void computeTouchTextOpacity();
		void renderIntro();
		void renderPuzzle();
		void renderHint();
		void renderValidation();
		void renderSuccess();
		void renderFail();
	};
};
