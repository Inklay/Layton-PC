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
		TITLE_SCREEN,
		CREATE_SAVE
	};

	Scene(Game* game);
	virtual void load();
	virtual void render() = 0;
	void handleEvent(SDL_Event event);
	virtual void handleClick(const std::string& spriteName) = 0;
	void unload();

	Game* m_game;
	uint64_t m_lastTick;

protected:
	void playBGM(const fileUtils::path& inputFile);
	void loopBGM();

	std::pair<std::string, Sprite*> m_clickedSprite;
	uint8_t* m_bgmBuffer;
	uint32_t m_bgmBufferLen;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
};
