#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class TitleDrop : public Scene {
	public:
		enum LogoState {
			FADING,
			TRANSLATING,
			DONE
		};

		TitleDrop(Game* game);
		void load();
		void render();
		void handleClick(const std::string&, SDL_Event);

	private:
		LogoState m_logoState = FADING;
		int m_touchCounter = -15;
	};

};
