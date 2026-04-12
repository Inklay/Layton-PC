#pragma once

#include "SDL3/SDL.h"

namespace sdlUtils {
	enum Dir {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	const int WIDTH = 256;
	const int HEIGHT = 384;
	const int HALF_HEIGHT = 192;

	float centerYPos(float size);
	float centerXPos(float size);
	float centerTopPos(float size);
	float centerBottomPos(float size);

	const SDL_AudioSpec audioSpec{ SDL_AUDIO_S16LE, 2, 16364 };

	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);

	void SDLCALL audioCallback(void* userData, SDL_AudioStream* stream, int additionalAmount, int totalAmount);

	struct AudioData {
		AudioData();

		SDL_AudioSpec bgmSpec;
		uint32_t bgmBufferLen;
		uint8_t* bgmBuffer;
	};
}
