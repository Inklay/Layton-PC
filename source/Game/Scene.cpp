#include "Game/Scene.h"
#include "Game/Game.h"

Scene::Scene(Game* game) :
	m_game(game),
	m_lastTick(0),
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

	if (m_game->m_audioData.bgmBuffer == nullptr) {
		SDL_free(m_game->m_audioData.bgmBuffer);
	}

	m_faded = false;
	m_fading = false;
	m_fadeProgress = 0;

	m_sprites.clear();

	SDL_ClearAudioStream(m_game->m_audioStream);
}

void Scene::playBGM(const fileUtils::path& inputFile) {
	//uint8_t* buffer;

	if (!SDL_LoadWAV(inputFile.string().c_str(), &m_game->m_audioData.bgmSpec, &m_game->m_audioData.bgmBuffer, &m_game->m_audioData.bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	SDL_SetAudioStreamFormat(m_game->m_audioStream, &m_game->m_audioData.bgmSpec, &m_game->m_audioData.bgmSpec);
	//m_game->m_audioData.bgmBuffer = (uint8_t*)SDL_malloc(m_game->m_audioData.bgmBufferLen);
	//memset(m_game->m_audioData.bgmBuffer, 0, m_game->m_audioData.bgmBufferLen);
	//SDL_MixAudio(m_game->m_audioData.bgmBuffer, buffer, m_game->m_audioData.bgmSpec.format, m_game->m_audioData.bgmBufferLen, 1.0f);

	if (!SDL_PutAudioStreamData(m_game->m_audioStream, m_game->m_audioData.bgmBuffer, m_game->m_audioData.bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
	}

	//SDL_free(buffer);
	SDL_ResumeAudioStreamDevice(m_game->m_audioStream);
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

	//if (m_bgmBuffer != nullptr) {
	//	int bufferLen = SDL_GetAudioStreamQueued(m_game->m_audioStream);
	//	uint8_t* buffer = (uint8_t*)SDL_malloc(bufferLen);

	//	memset(buffer, 0, bufferLen);
	//	SDL_MixAudio(buffer, m_bgmBuffer, m_bgmSpec.format, bufferLen, 1.0f - m_fadeProgress / 300);
	//	SDL_ClearAudioStream(m_game->m_audioStream);
	//	SDL_PutAudioStreamData(m_game->m_audioStream, buffer, bufferLen);
	//	SDL_free(buffer);
	//}

	SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
	SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, (uint8_t)opacity);
	SDL_RenderFillRect(m_game->m_renderer, NULL);
}
