#pragma once

#include "Game/Layton1/Scene/Map/Map.h"

namespace Layton1Scene {
	class DrawBridge : public Map {
	public:
		DrawBridge(Game* game);
		void handleClick(const std::string& spriteName, SDL_Event event);

	protected:
		void renderMap();
		void renderDialogue();
		void handleDialogProgression();
		void loadMap();
		void loadDialogue();
	};
};
