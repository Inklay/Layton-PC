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

	if (m_game->m_bgmData.buffer != nullptr) {
		m_game->m_bgmData.buffer = nullptr;
		m_game->m_bgmData.position = 0;
		m_game->m_bgmData.fading = false;
		m_game->m_bgmData.volume = 1.0f;
		m_game->m_bgmData.fadeProression = 0;
		SDL_free(m_game->m_bgmData.buffer);
	}

	if (m_sfxBuffer != nullptr) {
		SDL_free(m_sfxBuffer);
	}

	m_faded = false;
	m_fading = false;
	m_fadeProgress = 0;
	m_sprites.clear();

	SDL_ClearAudioStream(m_game->m_bgmStream);
}

void Scene::playBGM(const fileUtils::path& inputFile) {
	SDL_LoadWAV(inputFile.string().c_str(), &m_game->m_bgmData.spec, &m_game->m_bgmData.buffer, &m_game->m_bgmData.bufferLen);
	SDL_SetAudioStreamFormat(m_game->m_bgmStream, &m_game->m_bgmData.spec, &m_game->m_bgmData.spec);
	SDL_ResumeAudioStreamDevice(m_game->m_bgmStream);
}

void Scene::playSFX(const std::string& name) {
	SDL_AudioSpec spec;
	uint32_t bufferLen;

	if (m_sfxBuffer != nullptr) {
		SDL_free(m_sfxBuffer);
	}

	SDL_LoadWAV((m_game->m_gameFolder / "sound/sfx" / (m_game->m_sfx.at(name) + ".wav")).string().c_str(), &spec, &m_sfxBuffer, &bufferLen);
	SDL_SetAudioStreamFormat(m_game->m_sfxStream, &spec, &spec);
	SDL_ClearAudioStream(m_game->m_sfxStream);
	SDL_PutAudioStreamData(m_game->m_sfxStream, m_sfxBuffer, bufferLen);
	SDL_ResumeAudioStreamDevice(m_game->m_sfxStream);
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

void Scene::fadeOut() {
	size_t elapsedTime = SDL_GetTicks() - m_lastTick;
	m_fadeProgress += elapsedTime;
	int opacity = (int)((m_fadeProgress * 255) / (m_findOutSeconds * 1000));

	if (opacity >= 255) {
		m_faded = true;
		opacity = 255;
	}

	SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
	SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, (uint8_t)opacity);
	SDL_RenderFillRect(m_game->m_renderer, NULL);
}

void Scene::fadeIn() {
	if (m_lastTick == 0) {
		m_fadeProgress = 0;
	} else {
		size_t elapsedTime = SDL_GetTicks() - m_lastTick;
		m_fadeProgress += elapsedTime;
	}

	int opacity = 255 - (int)(((int)m_fadeProgress * 255) / (m_findInSeconds * 1000));

	if (opacity <= 0) {
		m_faded = true;
		m_fading = false;
		m_fadeProgress = 0;
		opacity = 0;
		return;
	}

	SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
	SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, (uint8_t)opacity);
	SDL_RenderFillRect(m_game->m_renderer, NULL);
}

void Scene::fadeToNextScene(Type type) {
	m_nextScene = Scene::CREATE_SAVE;
	m_fading = true;
	m_game->m_bgmData.fading = true;
	m_game->m_bgmData.fadingSeconds = m_findOutSeconds;
}

void Scene::fade() {
	if (m_fading) {
		if (m_nextScene == UNKNOWN) {
			fadeIn();
		} else {
			fadeOut();

			if (m_faded) {
				m_game->changeScene(m_nextScene);
			}
		}
	}
}
