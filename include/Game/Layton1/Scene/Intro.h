#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class Intro : public Scene {
	public:
		Intro(Game* game);
		void load();
		void render();
		void handleClick(const std::string&, SDL_Event) {};

	private:
		int m_step = 0;
		size_t m_timer = 0;
		uint8_t m_bottomOpacity = 0;
		SDL_FRect m_bottomScreenRect;
	};
};
