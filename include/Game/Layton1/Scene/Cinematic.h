#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class Cinematic : public Scene {
	public:
		Cinematic(Game* game, const fileUtils::path& videoFile, const fileUtils::path& audioFile);
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event) {};
	
	private:
		const fileUtils::path m_audioFile;
	};
};
