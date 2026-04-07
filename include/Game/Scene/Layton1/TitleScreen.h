#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

class TitleScreen : public Scene {
public:
	TitleScreen() : Scene(TITLE_SCREEN) {};
	void load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, float windowMultiplier);
	void render();
};

};
