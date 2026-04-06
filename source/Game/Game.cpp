#include "Game/Game.h"
#include "FileFormat/Conversion/LPCK.h"
#include "Utils/folderUtils.h"
#include "Utils/fileUtils.h"
#include "FileFormat/Conversion/BGX.h"
#include "FileFormat/Conversion/ARJ.h"
#include <array>
#include <algorithm>
#include <execution>

Game::Game(const fileUtils::path& assetsPath, const std::string& name, SDL_Window& window) :
	m_assetsPath(assetsPath),
	m_name(name),
	m_window(window)
{
	m_renderer = SDL_CreateRenderer(&m_window, nullptr);
	m_gameFolder = std::filesystem::current_path() / "games" / m_name;
	m_sceneType = Scene::TITLE_SCREEN;

	convertData();
}

void Game::convertData() const {
	/*if (std::filesystem::exists(m_gameFolder / "extracted")) {
		return;
	}*/

	if (!std::filesystem::exists(m_assetsPath)) {
		exit(1);
	}

	/*convertTextData();
	covertImgData();*/
	covertAnimData();

	fileUtils::writeText("", m_gameFolder / "extracted");
	std::cout << "Data conversion done !" << std::endl;
}

void Game::convertTextData() const {
	std::array<const fileUtils::path, 8> textFolders = {
		"etext",
		"htext",
		"itext",
		"otext",
		"qtext",
		"room/tobj",
		"stext",
		"storytext"
	};

	std::for_each(std::execution::par, textFolders.begin(), textFolders.end(),[this](const fileUtils::path& dir) {
		LPCK::convert(m_assetsPath / dir, m_gameFolder / dir);
	});
}

void Game::covertImgData() const {
	BGX::convert(m_assetsPath / "bg", m_gameFolder / "bg");
}

void Game::covertAnimData() const {
	ARJ::convert(m_assetsPath / "ani", m_gameFolder / "ani");
}

void Game::run() {
	bool isRunning = true;

	while (isRunning) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				isRunning = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);

		currentScene()->render(m_renderer);

		SDL_RenderPresent(m_renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(m_renderer);
}

void Game::changeScene(Scene::Type newScene) {
	currentScene()->unload();
	std::cout << m_scenes.count(newScene) << std::endl;
	Scene* scene = m_scenes.at(newScene).get();
	scene->load(m_gameFolder, m_renderer);
	m_sceneType = scene->type();
}

Scene* Game::currentScene() {
	return m_scenes.at(m_sceneType).get();
};
