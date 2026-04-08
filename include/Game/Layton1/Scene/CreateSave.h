#pragma once

#include "Game/Scene.h"

namespace Layton1Scene {

	class CreateSave : public Scene {
	public:
		CreateSave(Game* game);
		void load();
		void render();
		void handleClick(const std::string& spriteName);
	};

};
