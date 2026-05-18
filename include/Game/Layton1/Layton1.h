#pragma once

#include "Game/Game.h"

class Layton1 : public Game {
public:
	Layton1(const fileUtils::path& assetsPath, SDL_Window& window);
	void addSave(const std::u32string name);
	std::u32string getQuestText();

protected:
	void init();
	void getSaves();
};
