#include "Game/Scene.h"

Scene::Scene(Type type) :
	m_type(type)
{
	m_bgmBuffer = nullptr;
}

void Scene::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, SDL_AudioStream* audioStream, float windowMultiplier) {
	m_lastTick = 0;
	m_renderer = renderer;
	m_windowMultiplier = windowMultiplier;
	m_audioStream = audioStream;
}

void Scene::unload() {
	for (auto& it : m_sprites) {
		it.second->unload();
		it.second.release();
	}

	if (m_bgmBuffer == nullptr) {
		SDL_free(m_bgmBuffer);
	}

	m_sprites.clear();
}

Scene::Type Scene::type() const {
	return m_type;
}

void Scene::playBGM(const fileUtils::path& inputFile) {
	SDL_AudioSpec spec;
	uint8_t* buffer;

	if (!SDL_LoadWAV(inputFile.string().c_str(), &spec, &buffer, &m_bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	m_bgmBuffer = (uint8_t*)SDL_malloc(m_bgmBufferLen);
	memset(m_bgmBuffer, 0, m_bgmBufferLen);
	SDL_MixAudio(m_bgmBuffer, buffer, spec.format, m_bgmBufferLen, 0.2);

	if (!SDL_PutAudioStreamData(m_audioStream, m_bgmBuffer, m_bgmBufferLen)) {
		std::cerr << SDL_GetError() << std::endl;
	}

	SDL_free(buffer);
	SDL_ResumeAudioStreamDevice(m_audioStream);
}

void Scene::loopBGM() {
	if (m_bgmBuffer == nullptr) {
		return;
	}

	if (SDL_GetAudioStreamQueued(m_audioStream) < m_bgmBufferLen / 2) {
		SDL_PutAudioStreamData(m_audioStream, m_bgmBuffer, m_bgmBufferLen);
	}
}
