#pragma once

#include <string>
#include <map>
#include <SDL3/SDL.h>
#include "Utils/FileUtils.h"
#include "Game/Scene/Scene.h"
#include "Game/Save.h"

class Game {
public:
	Game(const fileUtils::path& assetsPath, const std::string& name, SDL_Window& window);
	void run();
	bool hasSave();
	void changeScene(const std::string& newScene);
	virtual void addSave(const std::u32string name) = 0;
	void setFrameDuration(int duration);
	virtual std::u32string getQuestText() = 0;
	
	const std::string m_name;
	std::string m_previousSceneName;
	std::vector<std::unique_ptr<Save>> m_saves;
	float m_windowMultiplier;
	std::vector<SDL_AudioStream*> m_bgmStreams;
	std::vector<SDL_AudioStream*> m_sfxStreams;
	Save* m_save;
	SDL_Window& m_window;
	SDL_Renderer* m_renderer;
	fileUtils::path m_gameFolder;
	std::vector<std::unique_ptr<sdlUtils::AudioData>> m_bgmData;
	std::map<std::string, std::string> m_sfx;

protected:
	virtual void init() = 0;
	virtual void getSaves() = 0;

	Scene* currentScene();
	std::map<std::string, std::unique_ptr<Scene>> m_scenes;

private:
	void convertData() const;
	void convertTextData() const;
	void convertImgData() const;
	void convertAnimData() const;
	void convertVideoData() const;
	void convertAudioData() const;
	void convertFontData() const;

	const fileUtils::path& m_assetsPath;
	std::string m_sceneName;
	int m_frameDuration = 16;
};
