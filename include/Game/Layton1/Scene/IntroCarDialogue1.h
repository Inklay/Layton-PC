#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class IntroCarDialogue1 : public Scene {
	public:
		IntroCarDialogue1(Game* game);
		void load();
		void render();
		void handleClick(const std::string&, SDL_Event);

	private:
		int m_dialogueProgression = 0;
		int m_topBackground = 0;
		int m_bottomBackground = 0;
	};
};
