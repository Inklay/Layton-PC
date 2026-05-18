#pragma once

#include "utils/fileUtils.h"
#include <SDL3/SDL.h>
#include "Game/Sprite/Sprite.h"

class Scene;

class Dialogue {
public:
	struct Character {
		std::string name;
		fileUtils::path nameSprite;
		fileUtils::path backgroundSprite;
		fileUtils::path noTalkAnim;
		fileUtils::path talkAnim;
		SDL_FRect rect;
		std::string mapSpriteName = "";
		bool talking = false;
		bool visible = false;
	};

	Dialogue(Scene* scene);
	void load(std::vector<Character> characters);
	void draw();
	void skip();
	bool next();
	bool fading();
	bool visible();
	bool waiting() const;
	void setVisible(bool visible);
	void setDialogue(const fileUtils::path& textFilePath, const std::string& character, std::vector<fileUtils::path> audioFilesPath = {}, const std::string& sfxSound = "charBip3");
	void setCharacterVisible(std::string name, bool visible);
	void fade(Sprite::FadeInfo fadeInfo);
	void hideAllCharacters();

private:
	void setAnimation();
	
	Scene* m_scene;
	std::vector<Character> m_characters;
	std::vector<std::u32string> m_texts;
	std::vector<fileUtils::path> m_audioFilesPath;
	std::u32string m_writtenText;
	std::string m_sfxSound = "charBip3";
	size_t m_textProgression = 0;
	size_t m_audioStreamIdx = -1;
	size_t m_currentText = 0;
	int m_delayCounter = 0;
	bool m_displayed = false;
	bool m_waiting = false;
};
