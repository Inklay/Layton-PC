#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class CreateSave : public Scene {
	public:
		enum KeyboardState {
			LOWER,
			UPPER,
			SHIFT,
			SPECIAL
		};

		CreateSave(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	private:
		bool isLetterClicked(float x, float y);

		KeyboardState m_keyboardState = LOWER;
		bool m_displayCursor = true;
		int m_cursorCount = 0;
	};

};
