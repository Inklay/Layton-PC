#include "Game/Layton1.h"
#include <iostream>

SDL_Window* initSdl() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL could not initialize! error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_Window* window = SDL_CreateWindow("Layton PC rewrite", 512, 768, 0);
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
