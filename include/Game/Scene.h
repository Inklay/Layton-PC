#pragma once

#include "Game/Sprite/Sprite.h"
#include <map>
#include <string>

class Game;

class Scene {
public:
	enum Type {
		EXTRACTING_DATA,
		INTRO,
		TITLE_SCREEN
	};

	Scene(Type type, Game* game);
	virtual void load();
	virtual void render() = 0;
	void unload();
	Type type() const;

	Game* m_game;
	uint64_t m_lastTick;

protected:
	void playBGM(const fileUtils::path& inputFile);
	void loopBGM();

	uint8_t* m_bgmBuffer;
	uint32_t m_bgmBufferLen;
	Type m_type;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
};
