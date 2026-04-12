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

	void SDLCALL bgmCallback(void* userData, SDL_AudioStream* stream, int additionalAmount, int totalAmount) {
		AudioData* audioData = (AudioData*)userData;

		if (audioData->buffer == nullptr) {
			return;
		}

		int available = audioData->bufferLen - audioData->position;
		int toWrite = std::min(additionalAmount, available);

		if (audioData->fading) {
			int bytesPerSample = SDL_AUDIO_BYTESIZE(audioData->spec.format) * audioData->spec.channels;
			int numSamples = toWrite / bytesPerSample;
			uint8_t* tmp = (uint8_t*)SDL_malloc(toWrite);

			SDL_memset(tmp, 0, toWrite);

			for (int i = 0; i < numSamples; i++) {
				int offset = i * bytesPerSample;

				SDL_MixAudio(tmp + offset, audioData->buffer + audioData->position + offset, audioData->spec.format, bytesPerSample, audioData->volume);
				audioData->volume -= 1.0f / (float)(audioData->spec.freq * audioData->fadingSeconds);

				if (audioData->volume < 0.0f) {
					audioData->volume = 0.0f;
					break;
				}
			}

			SDL_PutAudioStreamData(stream, tmp, toWrite);
			SDL_free(tmp);
		} else {
			SDL_PutAudioStreamData(stream, audioData->buffer + audioData->position, toWrite);
		}

		audioData->position += toWrite;

		if (audioData->position >= (int)audioData->bufferLen) {
			audioData->position = 0;
		}
	}
};
