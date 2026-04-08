#include "Utils/sdlUtils.h"
#include <iostream>
#include <cmath>

namespace sdlUtils {
	float scaleWindow(SDL_Window* window) {
		int h, w;

		const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
		SDL_GetWindowSize(window, &w, &h);
		float multiplier = std::min((float)displayMode->w / (float)WIDTH, (float)((displayMode->h * 0.9) / (float)HEIGHT));
		SDL_SetWindowSize(window, (int)(WIDTH * multiplier), (int)(HEIGHT * multiplier));
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		return multiplier;
	}

	void multiply(SDL_FRect& rect, float multiplier) {
		rect.h *= multiplier;
		rect.w *= multiplier;
		rect.x *= multiplier;
		rect.y *= multiplier;
	}

	float centerYPos(float size) {
		return std::round((HEIGHT - size) / 2);
	}

	float centerXPos(float size) {
		return std::round((WIDTH - size) / 2);
	}

	float centerTopPos(float size) {
		return std::round((HEIGHT / 2 - size) / 2);
	}

	float centerBottomPos(float size) {
		return std::round((HEIGHT * 1.5f - size) / 2);
	}
};
