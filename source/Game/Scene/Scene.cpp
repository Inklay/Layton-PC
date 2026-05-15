#include "Game/Scene/Scene.h"
#include "Game/Game.h"

Scene::Scene(Game* game, float fadeIn, float fadeOut) :
	m_game(game),
	m_lastTick(0),
	m_fading(false),
	m_faded(false),
	m_fadeProgress(0),
	m_nextScene("Unknown"),
	m_fadeInSeconds(fadeIn),
	m_fadeOutSeconds(fadeOut),
	m_dialogue(this),
	m_timeElapsed(0)
{
	m_sfxBuffers.emplace_back(nullptr);
}

void Scene::load() {
	m_lastTick = SDL_GetTicks();
	m_timeElapsed = 0;
}

void Scene::unload() {
	for (auto& it : m_sprites) {
		it.second->unload();
		it.second.release();
	}

	for (size_t i = m_game->m_bgmStreams.size() - 1; i != SIZE_MAX; i--) {
		clearAudioStream(m_game->m_bgmStreams.at(i), m_game->m_bgmData.at(i).get());

		if (i != 0) {
			SDL_DestroyAudioStream(m_game->m_bgmStreams.at(i));
			m_game->m_bgmStreams.pop_back();
			m_game->m_bgmData.pop_back();
		}
	}

	for (size_t i = m_game->m_sfxStreams.size() - 1; i != SIZE_MAX; i--) {
		if (SDL_GetAudioStreamQueued(m_game->m_sfxStreams.at(i))) {
			if (m_sfxBuffers.at(i) != nullptr) {
				SDL_free(m_sfxBuffers.at(i));
			}
		}

		if (i != 0) {
			SDL_DestroyAudioStream(m_game->m_sfxStreams.at(i));
			m_game->m_sfxStreams.pop_back();
			m_sfxBuffers.pop_back();
		}
	}

	m_faded = false;
	m_fading = false;
	m_fadeProgress = 0;
	m_sprites.clear();
}

void Scene::render() {
	fade();
	m_timeElapsed += SDL_GetTicks() - m_lastTick;
	m_lastTick = SDL_GetTicks();
}

void Scene::playBGM(const fileUtils::path& inputFile, size_t audioStreamIdx, bool loop) {
	if (m_game->m_bgmStreams.size() <= audioStreamIdx) {
		m_game->m_bgmData.emplace_back(std::make_unique<AudioData>());
		m_game->m_bgmData.at(audioStreamIdx)->loop = loop;
		m_game->m_bgmStreams.emplace_back(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, sdlUtils::bgmCallback, m_game->m_bgmData.at(audioStreamIdx).get()));
	}

	SDL_LoadWAV((m_game->m_gameFolder / inputFile).string().c_str(), &m_game->m_bgmData.at(audioStreamIdx)->spec, &m_game->m_bgmData.at(audioStreamIdx)->buffer, &m_game->m_bgmData.at(audioStreamIdx)->bufferLen);
	SDL_SetAudioStreamFormat(m_game->m_bgmStreams.at(audioStreamIdx), &m_game->m_bgmData.at(audioStreamIdx)->spec, &m_game->m_bgmData.at(audioStreamIdx)->spec);
	SDL_ResumeAudioStreamDevice(m_game->m_bgmStreams.at(audioStreamIdx));
}

void Scene::playSFX(const std::string& name, size_t audioStreamIdx) {
	SDL_AudioSpec spec;
	uint32_t bufferLen;

	if (m_game->m_sfxStreams.size() <= audioStreamIdx) {
		m_game->m_sfxStreams.emplace_back(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, nullptr));
		m_sfxBuffers.emplace_back(nullptr);
	}

	if (m_sfxBuffers.at(audioStreamIdx) != nullptr) {
		SDL_free(m_sfxBuffers.at(audioStreamIdx));
	}

	SDL_LoadWAV((m_game->m_gameFolder / "sound/sfx" / (m_game->m_sfx.at(name) + ".wav")).string().c_str(), &spec, &m_sfxBuffers.at(audioStreamIdx), &bufferLen);
	SDL_SetAudioStreamFormat(m_game->m_sfxStreams.at(audioStreamIdx), &spec, &spec);
	SDL_ClearAudioStream(m_game->m_sfxStreams.at(audioStreamIdx));
	SDL_PutAudioStreamData(m_game->m_sfxStreams.at(audioStreamIdx), m_sfxBuffers.at(audioStreamIdx), bufferLen);
	SDL_ResumeAudioStreamDevice(m_game->m_sfxStreams.at(audioStreamIdx));
}

void Scene::handleEvent(SDL_Event event) {
	if (m_fading) {
		return;
	}

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
	int opacity = (int)((m_fadeProgress * 255) / (m_fadeOutSeconds * 1000));

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

	int opacity = 255 - (int)(((int)m_fadeProgress * 255) / (m_fadeInSeconds * 1000));

	if (opacity <= 0) {
		m_fading = false;
		m_fadeProgress = 0;
		opacity = 0;
		return;
	}

	SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
	SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, (uint8_t)opacity);
	SDL_RenderFillRect(m_game->m_renderer, NULL);
}

void Scene::fadeToNextScene(const std::string& name) {
	m_nextScene = name;
	m_fading = true;

	for (auto& i : m_game->m_bgmData) {
		i->fading = true;
		i->fadingSeconds = m_fadeOutSeconds;
	}
}

void Scene::fade() {
	if (m_fading) {
		if (m_nextScene == "Unknown") {
			fadeIn();
		} else {
			fadeOut();

			if (m_faded) {
				m_game->changeScene(m_nextScene);
			}
		}
	}
}

void Scene::pauseBGM(size_t audioStreamIdx) {
	SDL_PauseAudioStreamDevice(m_game->m_bgmStreams.at(audioStreamIdx));
}

void Scene::fadeBGM(float duration, size_t audioStreamIdx) {
	m_game->m_bgmData.at(audioStreamIdx)->fading = true;
	m_game->m_bgmData.at(audioStreamIdx)->fadingSeconds = duration;
}

void Scene::switchSprites(const std::string& out, const std::string& in) {
	m_sprites.at(in)->fade(Sprite::FadeInfo{ 100, 0, Sprite::FadingMode::IN });
	m_sprites.at(out)->fade(Sprite::FadeInfo{ 100, 0, Sprite::FadingMode::OUT });
}

std::vector<fileUtils::path> Scene::getNumberSprites(int number, const std::string& font, int padding) {
	std::vector<fileUtils::path> vec;
	int digit;

	if (number == 0) {
		vec.emplace_back("ani/" + font + ".0.png");
		return vec;
	}

	while (number != 0) {
		digit = number % 10;
		vec.emplace_back("ani/" + font + "." + std::to_string(digit) + ".png");
		number /= 10;
	}

	while (vec.size() < padding) {
		vec.emplace_back("ani/" + font + ".0.png");
	}

	return vec;
}
