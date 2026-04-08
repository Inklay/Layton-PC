#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

class TitleScreen : public Scene {
public:
	TitleScreen(Game* game) : Scene(TITLE_SCREEN, game) {};
	void load();
	void render();
};

};
