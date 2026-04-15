#include "Game/Layton1/Scene/TitleScreen.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	TitleScreen::TitleScreen(Game* game) :
		Scene(game),
		m_hasSave(game->hasSave())
	{
	}

	void TitleScreen::load() {
		Scene::load();

		float startButtonYOffset = m_hasSave ? 50.0f : 30.0f;

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/select_title.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "gameLogo", std::make_unique<FadingSprite>("ani/fr/sub_title_logo.png", this, SDL_FRect{ centerXPos(168), centerTopPos(80), 168, 80}, 800, -1500)});
		m_sprites.insert({ "parallax1", std::make_unique<ParallaxSprite>("bg/start_select2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, 192 }, 10000, RIGHT) });
		m_sprites.insert({ "parallax2", std::make_unique<ParallaxSprite>("bg/start_select.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, 5000, RIGHT) });
		m_sprites.insert({ "car", std::make_unique<AnimatedSprite>("ani/start_car.gfx.anim", this, SDL_FRect{ centerXPos(98), 321, 98, 62})});
		m_sprites.insert({ "startButton", std::make_unique<ClickableSprite>("ani/fr/startbutton.0.png", "ani/fr/startbutton.1.png", this, SDL_FRect{ centerXPos(82), centerBottomPos(16) - startButtonYOffset, 82, 16})});

		playBGM("sound/SEQ_BG_001.wav");
		m_fadeOutSeconds = 0.8f;
	}

	void TitleScreen::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("gameLogo")->draw();
		m_sprites.at("parallax1")->draw();
		m_sprites.at("parallax2")->draw();
		m_sprites.at("car")->draw();
		m_sprites.at("startButton")->draw();

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void TitleScreen::handleClick(const std::string& spriteName, SDL_Event event) {
		if (spriteName == "startButton") {
			playSFX("titleScreenClick");
			fadeToNextScene(CREATE_SAVE);
		}
	}
};
