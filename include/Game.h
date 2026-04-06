#pragma once

#include <string>
#include "Utils/FileUtils.h"
#include "Utils/SdlUtils.h"

class Game {
public:
	Game(const fileUtils::path& assetsPath, const std::string& name, sdlUtils::SDL_WindowPtr window);
	void run() const;

private:
	void convertData() const;
	void convertTextData() const;
	void covertImgData() const;

	const fileUtils::path& m_assetsPath;
	const std::string& m_name;
	fileUtils::path m_gameFolder;
	sdlUtils::SDL_WindowPtr m_window;
};
