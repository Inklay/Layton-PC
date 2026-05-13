#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class IntroCarDialogue2 : public Scene {
	public:
		IntroCarDialogue2(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	private:
		int m_dialogueProgression = 0;
		int m_topBackground = 0;
		int m_bottomBackground = 0;
	};
};
