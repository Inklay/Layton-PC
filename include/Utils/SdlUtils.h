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
	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);
	void SDLCALL bgmCallback(void* userData, SDL_AudioStream* stream, int additionalAmount, int totalAmount);

	struct AudioData {
		SDL_AudioSpec spec = { SDL_AUDIO_UNKNOWN, 0, 0 };
		uint32_t bufferLen = 0;
		int position = 0;
		uint8_t* buffer = nullptr;
		bool fading = false;
		float volume = 1.0f;
		int fadeProression = 0;
	};
}
