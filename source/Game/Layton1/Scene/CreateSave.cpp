#include "Game/Layton1/Scene/CreateSave.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	CreateSave::CreateSave(Game* game) :
		Scene(game)
	{
	}

	void CreateSave::load() {
		Scene::load();
	}

	void CreateSave::render() {
		loopBGM();

		m_lastTick = SDL_GetTicks();
	}

	void CreateSave::handleClick(const std::string& spriteName) {
	}
};
