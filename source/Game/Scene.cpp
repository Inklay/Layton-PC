#include "Game/Scene.h"
#include "Game/Game.h"

Scene::Scene(Game* game) :
	m_game(game),
	m_lastTick(0),
	m_bgmBuffer(nullptr),
	m_bgmBufferLen(0),
	m_fading(false),
	m_faded(false),
	m_fadeProgress(0),
	m_nextScene(UNKNOWN)
{
}

void Scene::load() {
	m_lastTick = 0;
}

void Scene::unload() {
	for (auto& it : m_sprites) {
		it.second->unload();
		it.second.release();
	}

	if (m_bgmBuffer == nullptr) {
		SDL_free(m_bgmBuffer);
	}

	m_faded = false;
	m_fading = false;
	m_fadeProgress = 0;

	m_sprites.clear();
}

void Scene::playBGM(const fileUtils::path& inputFile) {
	SDL_AudioSpec spec;
	uint8_t* buffer;

	if (!SDL_LoadWAV(inputFile.string().c_str(), &spec, &buffer, &m_bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	SDL_SetAudioStreamFormat(m_game->m_audioStream, &spec, &spec);
	m_bgmBuffer = (uint8_t*)SDL_malloc(m_bgmBufferLen);
	memset(m_bgmBuffer, 0, m_bgmBufferLen);
	SDL_MixAudio(m_bgmBuffer, buffer, spec.format, m_bgmBufferLen, 1.0f);

	if (!SDL_PutAudioStreamData(m_game->m_audioStream, m_bgmBuffer, m_bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
	}

	SDL_free(buffer);
	SDL_ResumeAudioStreamDevice(m_game->m_audioStream);
}

void Scene::loopBGM() {
	if (m_bgmBuffer == nullptr) {
		return;
	}

	if ((uint32_t)SDL_GetAudioStreamQueued(m_game->m_audioStream) < m_bgmBufferLen / 2) {
		SDL_PutAudioStreamData(m_game->m_audioStream, m_bgmBuffer, m_bgmBufferLen);
	}
}

void Scene::handleEvent(SDL_Event event) {
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
		int maxZIndex = -1;
		for (auto& it : m_sprites) {
			if (it.second->m_interactive && it.second->isHovered() && it.second->m_zIndex > maxZIndex) {
				m_clickedSprite = { it.first, it.second.get()};
				maxZIndex = it.second->m_zIndex;
			}
		}
		if (m_clickedSprite.second != nullptr) {
			m_clickedSprite.second->setClicked(true);
		}
	} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT) {
		if (m_clickedSprite.second == nullptr) {
			return;
		}

		m_clickedSprite.second->setClicked(false);
		if (m_clickedSprite.second->isHovered()) {
			handleClick(m_clickedSprite.first, event);
			m_clickedSprite.second = nullptr;
		}
	}
}

void Scene::fadeToBlack() {
	size_t elapsedTime = SDL_GetTicks() - m_lastTick;
	m_fadeProgress += elapsedTime;
	size_t opacity = (m_fadeProgress * 255) / 300;

	if (opacity >= 255) {
		m_faded = true;
		opacity = 255;
	}

	SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
	SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, (uint8_t)opacity);
	SDL_RenderFillRect(m_game->m_renderer, NULL);
}
