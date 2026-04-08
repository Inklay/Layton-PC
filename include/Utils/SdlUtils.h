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

	float centerYPos(float size);
	float centerXPos(float size);
	float centerTopPos(float size);
	float centerBottomPos(float size);

	const SDL_AudioSpec audioSpec{ SDL_AUDIO_S16LE, 2, 16364 };

	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);
}
