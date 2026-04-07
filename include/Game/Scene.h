#pragma once

#include "Game/Sprite/Sprite.h"
#include <map>
#include <string>

class Scene {
public:
	enum Type {
		EXTRACTING_DATA,
		INTRO,
		TITLE_SCREEN
	};

	Scene(Type type);
	virtual void load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, SDL_AudioStream* audioStream, float windowMultiplier);
	virtual void render() = 0;
	void unload();
	Type type() const;


	uint64_t m_lastTick;
	SDL_Renderer* m_renderer;
	SDL_AudioStream* m_audioStream;
	float m_windowMultiplier;

protected:
	void playBGM(const fileUtils::path& inputFile);
	void loopBGM();

	uint8_t* m_bgmBuffer;
	uint32_t m_bgmBufferLen;
	Type m_type;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
};
