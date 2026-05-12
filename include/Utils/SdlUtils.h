#pragma once

#include <SDL3/SDL.h>

namespace sdlUtils {
	enum Dir {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	struct AudioData {
		SDL_AudioSpec spec = { SDL_AUDIO_UNKNOWN, 0, 0 };
		uint32_t bufferLen = 0;
		int position = 0;
		float volume = 1.0f;
		uint8_t* buffer = nullptr;
		int fadeProression = 0;
		float fadingSeconds = 1.0f;
		bool fading = false;
		bool loop = true;
		bool finished = false;
	};

	inline const int WIDTH = 256;
	inline const int HEIGHT = 384;
	inline const int HALF_HEIGHT = 192;

	float centerYPos(float size);
	float centerXPos(float size);
	float centerTopPos(float size);
	float centerBottomPos(float size);
	float scaleWindow(SDL_Window* window);
	void multiply(SDL_FRect& rect, float multiplier);
	void clearAudioStream(SDL_AudioStream* stream, sdlUtils::AudioData* audioData);
	void SDLCALL bgmCallback(void* userData, SDL_AudioStream* stream, int additionalAmount, int totalAmount);
	void renderLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float thickness, SDL_FColor color);
	void renderFilledCircle(SDL_Renderer* renderer, float cx, float cy, float radius, SDL_FColor color);
}
