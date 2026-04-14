#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class Intro : public Scene {
	public:
		Intro(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	private:
		int m_step = 0;
		size_t m_timer = 0;
		uint8_t m_bottomOpacity = 0;
		SDL_FRect m_bottomScreenRect;
	};
};
