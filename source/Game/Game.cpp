#include "Game/Game.h"
#include "FileFormat/Conversion/LPCK.h"
#include "Utils/folderUtils.h"
#include "Utils/fileUtils.h"
#include "FileFormat/Conversion/BGX.h"
#include "FileFormat/Conversion/GFX.h"
#include "FileFormat/Conversion/MODS.h"
#include "FileFormat/Conversion/SADL.h"
#include "FileFormat/Conversion/SDAT.h"
#include "FileFormat/Conversion/NFTR.h"
#include "Utils/sdlutils.h"
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

	m_windowMultiplier = sdlUtils::scaleWindow(&window);
	m_audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &sdlUtils::audioSpec, NULL, nullptr);

	if (m_audioStream == NULL) {
		std::cerr << SDL_GetError() << std::endl;
	}

	convertData();
}

void Game::convertData() const {
	if (std::filesystem::exists(m_gameFolder / "extracted")) {
		return;
	}

	if (!std::filesystem::exists(m_assetsPath)) {
		std::cerr << "can't open " << std::filesystem::absolute(m_assetsPath) << std::endl;
		exit(1);
	}

	convertTextData();
	convertImgData();
	convertAnimData();
	convertVideoData();
	convertAudioData();
	convertFontData();

	fileUtils::writeText("", m_gameFolder / "extracted");
	std::cout << "Data conversion done !" << std::endl;
}

void Game::convertTextData() const {
	std::cout << "Converting text data..." << std::endl;

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

void Game::convertImgData() const {
	std::cout << "Converting image data..." << std::endl;

	BGX::convert(m_assetsPath / "bg", m_gameFolder / "bg");
}

void Game::convertAnimData() const {
	std::cout << "Converting animation data..." << std::endl;

	GFX::convert(m_assetsPath / "ani", m_gameFolder / "ani");
}

void Game::convertVideoData() const {
	std::cout << "Converting video data..." << std::endl;

	MODS::convert(m_assetsPath / "video", m_gameFolder / "video");
}

void Game::convertAudioData() const {
	std::cout << "Converting audio data..." << std::endl;

	SADL::convert(m_assetsPath / "stream", m_gameFolder / "stream");
	SDAT::convert(m_assetsPath / "sound", m_gameFolder / "sound");
}

void Game::convertFontData() const {
	std::cout << "Converting font data..." << std::endl;

	NFTR::convert(m_assetsPath / "font", m_gameFolder / "font");
}

void Game::run() {
	bool isRunning = true;

	while (isRunning) {
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		currentScene()->render();
		SDL_RenderPresent(m_renderer);
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				isRunning = false;
				break;
			}

			currentScene()->handleEvent(event);
		}

		SDL_Delay(16);
	}

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyAudioStream(m_audioStream);
}

void Game::changeScene(Scene::Type newScene) {
	currentScene()->unload();
	Scene* scene = m_scenes.at(newScene).get();
	scene->load();
	m_sceneType = newScene;
}

Scene* Game::currentScene() {
	return m_scenes.at(m_sceneType).get();
};

bool Game::hasSave() {
	for (int i = 0; i < 3; i++) {
		if (m_saves.at(i).get() != nullptr) {
			return true;
		}
	}

	return false;
}
