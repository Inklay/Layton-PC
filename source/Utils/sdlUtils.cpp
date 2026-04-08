#include "Utils/sdlUtils.h"
#include <iostream>
#include <cmath>

namespace sdlUtils {
	float scaleWindow(SDL_Window* window) {
		int h, w;

		const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
		SDL_GetWindowSize(window, &w, &h);
		float multiplier = std::min((float)displayMode->w / (float)baseWidth, (float)((displayMode->h * 0.9) / (float)baseHeight));
		SDL_SetWindowSize(window, (int)(baseWidth * multiplier), (int)(baseHeight * multiplier));
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
		return (baseHeight- size) / 2;
	}

	float centerXPos(float size) {
		return (baseWidth - size) / 2;
	}

	float centerTopPos(float size) {
		return (baseHeight / 2 - size) / 2;
	}

	float centerBottomPos(float size) {
		return (baseHeight * 1.5f - size) / 2;
	}
};
