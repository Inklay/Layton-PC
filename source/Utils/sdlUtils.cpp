#include "Utils/sdlUtils.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <numbers>

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

	void clearAudioStream(SDL_AudioStream* stream, AudioData* audioData) {
		audioData->bufferLen = 0;
		audioData->position = 0;
		audioData->fading = false;
		audioData->volume = 1.0f;
		audioData->fadeProression = 0;
		audioData->finished = false;

		if (audioData->buffer != nullptr) {
			SDL_free(audioData->buffer);
			audioData->buffer = nullptr;
		}

		SDL_ClearAudioStream(stream);
	}

	void SDLCALL bgmCallback(void* userData, SDL_AudioStream* stream, int additionalAmount, int) {
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
			if (audioData->loop) {
				audioData->position = 0;
			} else {
				audioData->finished = true;
			}
		}
	}

	void renderLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float thickness, SDL_FColor color) {
		float dx = x2 - x1;
		float dy = y2 - y1;
		float len = sqrtf(dx * dx + dy * dy);

		if (len < 0.001f) {
			return;
		}

		float nx = (-dy / len) * (thickness * 0.5f);
		float ny = (dx / len) * (thickness * 0.5f);

		SDL_Vertex verts[4] = {
			{ {x1 + nx, y1 + ny}, color, {} },
			{ {x1 - nx, y1 - ny}, color, {} },
			{ {x2 + nx, y2 + ny}, color, {} },
			{ {x2 - nx, y2 - ny}, color, {} }
		};

		int indices[] = { 0, 1, 2,  1, 2, 3 };

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderGeometry(renderer, NULL, verts, 4, indices, 6);
	}

	void renderFilledCircle(SDL_Renderer* renderer, float cx, float cy, float radius, SDL_FColor color) {
		int segments = 20;
		std::vector<SDL_Vertex> verts;
		std::vector<int> indices;

		SDL_Vertex center;
		center.position = { cx, cy };
		center.color = color;
		center.tex_coord = { 0, 0 };
		verts.push_back(center);

		for (int i = 0; i <= segments; i++) {
			float angle = (2.0f * std::numbers::pi_v<float> * i) / segments;
			SDL_Vertex v;

			v.position = { cx + cosf(angle) * radius, cy + sinf(angle) * radius };
			v.color = color;
			v.tex_coord = { 0, 0 };
			verts.push_back(v);
		}

		for (int i = 1; i <= segments; i++) {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		SDL_RenderGeometry(renderer, NULL, verts.data(), (int)verts.size(), indices.data(), (int)indices.size());
	}
};
