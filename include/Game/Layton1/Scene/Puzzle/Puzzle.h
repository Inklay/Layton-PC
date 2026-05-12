#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class Puzzle : public Scene {
	public:
		Puzzle(Game* game, int picarat, int number, const std::string& internalName);
		void load();
		void render();
		void unload();
		void handleClick(const std::string& spriteName, SDL_Event event);

	protected:
		std::vector<fileUtils::path> getNumberSprites(int number, const std::string& font, int padding = 0);
		void hideBottomUI();
		void showBottomUI();
		bool isClosedShape(std::vector<SDL_FPoint> points);
		SDL_FPoint getShapeCenter(std::vector<SDL_FPoint> points);

		std::string m_internalName;
		std::u32string m_text;
		int m_picarat;
		int m_currentPicarat;
		int m_number;
		int m_textProgression = 0;
		int m_currentHint = 0;
		bool m_isIntro = true;
		bool m_movedTitleCard = false;
		bool m_fadingToPuzzle = false;
		bool m_canClear = false;
		bool m_canValidate = false;
		bool m_bottomUIHidden = false;
		bool m_displayHint = false;
	};
};
