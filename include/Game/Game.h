#pragma once

#include <string>
#include <map>
#include "Utils/FileUtils.h"
#include "SDL3/SDL.h"
#include "Game/Scene.h"
#include "Game/Save.h"

class Game {
public:
	Game(const fileUtils::path& assetsPath, const std::string& name, SDL_Window& window);
	void run();
	bool hasSave();
	void changeScene(Scene::Type newScene);
	
	std::vector<std::unique_ptr<Save>> m_saves;
	float m_windowMultiplier;
	SDL_AudioStream* m_bgmStream;
	SDL_AudioStream* m_sfxStream;
	std::unique_ptr<Save> m_save;
	SDL_Window& m_window;
	SDL_Renderer* m_renderer;
	fileUtils::path m_gameFolder;
	sdlUtils::AudioData m_bgmData;
	sdlUtils::AudioData m_sfxData;

protected:
	virtual void createScenes() = 0;
	virtual void getSaves() = 0;

	Scene* currentScene();
	std::map<Scene::Type, std::unique_ptr<Scene>> m_scenes;
	const std::string& m_name;

private:
	void convertData() const;
	void convertTextData() const;
	void convertImgData() const;
	void convertAnimData() const;
	void convertVideoData() const;
	void convertAudioData() const;
	void convertFontData() const;

	const fileUtils::path& m_assetsPath;
	Scene::Type m_sceneType;
};
