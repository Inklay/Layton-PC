#include "Game/Scene/Layton1/TitleScreen.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, float windowMultiplier) {
		m_lastTick = 0;
		m_renderer = renderer;
		m_windowMultiplier = windowMultiplier;

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>(assetFolder / "bg/select_title.png", this, SDL_FRect{ 0, 0, 256, 198}) });
		m_sprites.insert({ "gameLogo", std::make_unique<FadingSprite>(assetFolder / "ani/fr/title_logo.png", this, SDL_FRect{ 43, 56, 170, 74}, 500, -1500) });
		m_sprites.insert({ "parallax1", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select2.png", this, SDL_FRect{ 0, 198, 256, 198 }, 13000, sdlUtils::RIGHT) });
		m_sprites.insert({ "parallax2", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select.png", this, SDL_FRect{ 0, 198, 256, 198 }, 5000, sdlUtils::RIGHT) });
		m_sprites.insert({ "car", std::make_unique<AnimatedSprite>(assetFolder / "ani/start_car.gfx.anim", this, SDL_FRect{ SDL_WINDOWPOS_CENTERED, 320, 98, 62}) });
	}

	void TitleScreen::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("gameLogo")->draw();
		m_sprites.at("parallax1")->draw();
		m_sprites.at("parallax2")->draw();
		m_sprites.at("car")->draw();

		m_lastTick = SDL_GetTicks();
	}
};
