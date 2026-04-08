#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

class TitleScreen : public Scene {
public:
	TitleScreen(Game* game);
	void load();
	void render();
	void handleClick(const std::string& spriteName);

private:
	bool m_hasSave;
};

};
