#include "Utils/sdlUtils.h"
#include <iostream>
#include <cmath>

namespace sdlUtils {
	float scaleWindow(SDL_Window* window) {
		int h, w;

		const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
		SDL_GetWindowSize(window, &w, &h);
		float multiplier = std::min((float)displayMode->w / (float)baseWidth, (float)((displayMode->h * 0.9) / (float)baseHeight));
		SDL_SetWindowSize(window, baseWidth * multiplier, baseHeight * multiplier);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		return multiplier / 2;
	}

	void multiply(SDL_FRect& rect, float multiplier) {
		rect.h *= multiplier;
		rect.w *= multiplier;
		rect.x *= multiplier;
		rect.y *= multiplier;
	}
};
