#include "Game.h"
#include "FileFormat/Conversion/LPCK.h"
#include "Utils/folderUtils.h"
#include "Utils/fileUtils.h"
#include "FileFormat/Conversion/BG.h"
#include <array>
#include <algorithm>
#include <execution>

Game::Game(const fileUtils::path& assetsPath, const std::string& name, sdlUtils::SDL_WindowPtr window) :
	m_assetsPath(assetsPath),
	m_name(name),
	m_window(std::move(window))
{
	m_gameFolder = std::filesystem::current_path() / "games" / m_name;
	convertData();
}

void Game::convertData() const {
	/*if (std::filesystem::exists(m_gameFolder / "extracted")) {
		return;
	}*/

	if (!std::filesystem::exists(m_assetsPath)) {
		exit(1);
	}

	convertTextData();
	covertImgData();

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
	BG::convert(m_assetsPath / "bg", m_gameFolder / "bg");
}

void Game::run() const {
	while (1);
}
