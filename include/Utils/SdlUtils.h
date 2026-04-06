#pragma once

#include "SDL3/SDL.h"
#include <memory>

namespace sdlUtils {
	struct SDL_Deleter {
		void operator()(SDL_Window* ptr);
	};

	using SDL_WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
};
