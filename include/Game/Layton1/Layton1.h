#pragma once

#include "Game/Game.h"

class Layton1 : public Game {
public:
	Layton1(const fileUtils::path& assetsPath, SDL_Window& window);

protected:
	void init();
	void getSaves();
};
