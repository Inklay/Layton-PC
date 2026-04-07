#pragma once

#include "SDL3/SDL.h"

namespace sdlUtils {
	const int baseWidth = 256;
	const int baseHeight = 384;

	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);
}
