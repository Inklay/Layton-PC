#pragma once

#include "SDL3/SDL.h"

namespace sdlUtils {
	enum Dir {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	const int baseWidth = 256;
	const int baseHeight = 384;

	const int POS_CENTER = 0xFFFFFF00;
	const int POS_CENTER_TOP = 0xFFFFFF01;
	const int POS_CENTER_BOTTOM = 0xFFFFFF02;

	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);
}
