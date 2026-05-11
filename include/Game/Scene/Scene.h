#pragma once

#include "Game/Sprite/Sprite.h"
#include "Game/Scene/Dialogue.h"
#include <map>
#include <string>

using namespace sdlUtils;

class Game;

class Scene {

friend class Dialogue;

public:
	Scene(Game* game, float fadeIn = 0.0f, float fadeOut = 0.0f);
	virtual void load();
	virtual void render();
	virtual void unload();
	void handleEvent(SDL_Event event);
	virtual void handleClick(const std::string& spriteName, SDL_Event event) = 0;

	Game* m_game;
	uint64_t m_lastTick;
	bool m_fading;
	bool m_faded;

protected:
	void pauseBGM();
	void playBGM(const fileUtils::path& inputFile, size_t audioStreamIdx = 0, bool loop = true);
	void playSFX(const std::string& name);
	void fadeToNextScene(const std::string& name);
	void fade();

	std::pair<std::string, Sprite*> m_clickedSprite;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
	size_t m_fadeProgress;
	std::string m_nextScene;
	Dialogue m_dialogue;
	uint64_t m_timeElapsed;
	float m_fadeOutSeconds;
	float m_fadeInSeconds;

private:
	void fadeOut();
	void fadeIn();

	uint8_t* m_sfxBuffer;
};
