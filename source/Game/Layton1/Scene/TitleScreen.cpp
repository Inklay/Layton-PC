#include "Game/Layton1/Scene/TitleScreen.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, SDL_AudioStream* audioStream, float windowMultiplier) {
		Scene::load(assetFolder, renderer, audioStream, windowMultiplier);

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>(assetFolder / "bg/select_title.png", this, SDL_FRect{ 0, 0, 256, 192 }) });
		m_sprites.insert({ "gameLogo", std::make_unique<FadingSprite>(assetFolder / "ani/fr/sub_title_logo.png", this, SDL_FRect{ sdlUtils::POS_CENTER, sdlUtils::POS_CENTER_TOP, 170, 74}, 800, -1500) });
		m_sprites.insert({ "parallax1", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select2.png", this, SDL_FRect{ 0, 192, 256, 192 }, 10000, sdlUtils::RIGHT) });
		m_sprites.insert({ "parallax2", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select.png", this, SDL_FRect{ 0, 192, 256, 192 }, 5000, sdlUtils::RIGHT) });
		m_sprites.insert({ "car", std::make_unique<AnimatedSprite>(assetFolder / "ani/start_car.gfx.anim", this, SDL_FRect{ sdlUtils::POS_CENTER, 321, 98, 62}) });

		playBGM(assetFolder / "stream/BGM_09.wav");
	}

	void TitleScreen::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("gameLogo")->draw();
		m_sprites.at("parallax1")->draw();
		m_sprites.at("parallax2")->draw();
		m_sprites.at("car")->draw();

		loopBGM();

		m_lastTick = SDL_GetTicks();
	}
};
