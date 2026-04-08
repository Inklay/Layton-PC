#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

class TitleScreen : public Scene {
public:
	TitleScreen(Game* game);
	void load();
	void render();
	void handleClick(const std::string& spriteName, SDL_Event event);

private:
	bool m_hasSave;
};

};
