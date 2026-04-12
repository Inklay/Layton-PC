#pragma once

#include "Game/Sprite/Sprite.h"
#include <map>
#include <string>

using namespace sdlUtils;

class Game;

class Scene {
public:
	enum Type {
		UNKNOWN,
		EXTRACTING_DATA,
		INTRO,
		TITLE_SCREEN,
		CREATE_SAVE
	};

	Scene(Game* game);
	virtual void load();
	virtual void render() = 0;
	void handleEvent(SDL_Event event);
	virtual void handleClick(const std::string& spriteName, SDL_Event event) = 0;
	void unload();
	void fadeToBlack();

	Game* m_game;
	uint64_t m_lastTick;
	bool m_fading;
	bool m_faded;

protected:
	void playBGM(const fileUtils::path& inputFile);
	void playSFX(const fileUtils::path& inputFile);
	void fadeToNextScene(Type type);

	std::pair<std::string, Sprite*> m_clickedSprite;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
	size_t m_fadeProgress;
	Type m_nextScene;
};
