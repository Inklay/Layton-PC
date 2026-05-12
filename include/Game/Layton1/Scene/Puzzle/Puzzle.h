#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {

	class Puzzle : public Scene {
	public:
		Puzzle(Game* game, int picarat, int number, const fileUtils::path& nameFilePath);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	protected:
		std::vector<fileUtils::path> getNumberSprites(int number, const std::string& font, int padding = 0);

		int m_picarat;
		int m_currentPicarat;
		int m_number;
		fileUtils::path m_nameFilePath;
		bool m_isIntro = true;
		bool m_movedTitleCard = false;
	};
};
