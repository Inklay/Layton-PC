#pragma once

#include <string>
#include <map>
#include "Utils/FileUtils.h"
#include "SDL3/SDL.h"
#include "Game/Scene.h"

class Game {
public:
	Game(const fileUtils::path& assetsPath, const std::string& name, SDL_Window& window);
	void run();

protected:
	void changeScene(Scene::Type newScene);
	virtual void createScenes() = 0;

	SDL_Window& m_window;
	SDL_Renderer* m_renderer;
	fileUtils::path m_gameFolder;
	Scene* currentScene();
	std::map<Scene::Type, std::unique_ptr<Scene>> m_scenes;
	float m_windowMultiplier;

private:
	void convertData() const;
	void convertTextData() const;
	void convertImgData() const;
	void convertAnimData() const;
	void convertVideoData() const;
	void convertAudioData() const;

	const fileUtils::path& m_assetsPath;
	const std::string& m_name;
	Scene::Type m_sceneType;
};
