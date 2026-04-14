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
		DEVLOGO,
		TITLE_SCREEN,
		CREATE_SAVE,
		INTRO
	};

	Scene(Game* game);
	virtual void load();
	virtual void render() = 0;
	void handleEvent(SDL_Event event);
	virtual void handleClick(const std::string& spriteName, SDL_Event event) = 0;
	void unload();

	Game* m_game;
	uint64_t m_lastTick;
	bool m_fading;
	bool m_faded;

protected:
	void playBGM(const fileUtils::path& inputFile);
	void playSFX(const std::string& name);
	void fadeToNextScene(Type type);
	void fade();
	std::u32string getText(const fileUtils::path& inputFile, const std::u32string& toReplace = U"", const std::u32string& replaceWith = U"");

	std::pair<std::string, Sprite*> m_clickedSprite;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
	size_t m_fadeProgress;
	Type m_nextScene;
	float m_fadeOutSeconds;
	float m_fadeInSeconds;
	std::map<std::string, std::u32string> m_texts;

private:
	void fadeOut();
	void fadeIn();

	uint8_t* m_sfxBuffer;
};
