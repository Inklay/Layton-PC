#include "Game/Layton1/Scene/Intro.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Intro::Intro(Game* game) :
		Scene(game)
	{
	}

	void Intro::load() {
		Scene::load();

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/ara_top.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_20.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });

		m_fading = true;
		m_fadeInSeconds = 0.3f;
	}

	void Intro::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("bottomBackground")->draw();

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void Intro::handleClick(const std::string& spriteName, SDL_Event event) {
	}
};
