#pragma once

#include "utils/fileUtils.h"
#include <SDL3/SDL.h>

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
		bool talking = false;
	};

	Dialogue(Scene* scene);
	void load(std::vector<Character> characters);
	void draw();
	void setVisible(bool visible);
	void setDialogue(const fileUtils::path& textFilePath, const std::string& character, const fileUtils::path& audioFilePath);

private:
	Scene* m_scene;
	bool m_displayed = false;
	std::vector<Character> m_characters;
	std::u32string m_text;
	int m_textProgression = 0;
	int m_delayCounter = 0;
};
