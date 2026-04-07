#include "Game/Layton1.h"
#include <iostream>
#include "Utils/sdlUtils.h"

SDL_Window* initSdl() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "SDL could not initialize! error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_Window* window = SDL_CreateWindow("Layton PC rewrite", sdlUtils::baseWidth, sdlUtils::baseHeight, 0);
	if (window == nullptr) {
		std::cerr << "SDL could not created window! error: " << SDL_GetError() << std::endl;
	}

	return window;
}

int main()
{
	SDL_Window *window = initSdl();

	if (window == nullptr) {
		exit(1);
	}

	Layton1 game("NDS_UNPACK\\data\\data", *window);
	game.run();

	SDL_DestroyWindow(window);
	SDL_Quit();
}
