#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class StMystereEntry : public Scene {
	public:
		StMystereEntry(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	private:
		int m_dialogueProgression = 0;
		int m_bottomBackground = 0;
		int m_topBackground = 0;
	};
};
