#include "Game/Layton1/Scene/IntroCarDialogue1.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	IntroCarDialogue1::IntroCarDialogue1(Game* game) :
		Scene(game),
		m_hasSave(game->hasSave())
	{
	}

	void IntroCarDialogue1::load() {
		Scene::load();

		m_sprites.insert({ "topBackground", std::make_unique<FadingSprite>("bg/I_car4.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT}, 500, -1000) });
		m_lastTick = SDL_GetTicks();
	}

	void IntroCarDialogue1::render() {
		m_sprites.at("topBackground")->draw();

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void IntroCarDialogue1::handleClick(const std::string& spriteName, SDL_Event event) {
	}
};
