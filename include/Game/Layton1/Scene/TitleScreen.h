#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

class TitleScreen : public Scene {
public:
	TitleScreen(Game* game);
	void load();
	void render();

private:
	bool m_hasSave;
};

};
