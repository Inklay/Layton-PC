#include "LPCK.h"
#include "Game.h"
#include <iostream>
#include <memory>

sdlUtils::SDL_WindowPtr initSdl() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL could not initialize! error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	sdlUtils::SDL_WindowPtr window(SDL_CreateWindow("Layton PC rewrite", 640, 480, 0));
	if (window == nullptr) {
		std::cerr << "SDL could not created window! error: " << SDL_GetError() << std::endl;
	}

	return window;
}

int main()
{
	sdlUtils::SDL_WindowPtr window = initSdl();

	if (window.get() == nullptr) {
		std::string test;
		std::cin >> test;
		exit(1);
	}

	auto test = SDL_GetWindowSurface(window.get());

	const Game game("C:\\Users\\inkla\\Desktop\\NDS_UNPACK\\data\\data", "Curious Village", std::move(window));
	game.run();
}
