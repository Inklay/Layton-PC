#include "Game/Scene/Layton1/TitleScreen.h"
#include "Game/Sprite/ParallaxSprite.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, float windowMultiplier) {
		m_gameLogoOpacityTimer = -1500;
		m_lastTick = 0;
		m_renderer = renderer;
		m_windowMultiplier = windowMultiplier;

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>(assetFolder / "bg/select_title.png", this, SDL_FRect{ 0, 0, 256, 198}) });
		m_sprites.insert({ "gameLogo", std::make_unique<Sprite>(assetFolder / "ani/fr/title_logo.png", this, SDL_FRect{ 43, 56, 170, 74}) });
		m_sprites.insert({ "parallax1", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select2.png", this, SDL_FRect{ 0, 198, 256, 198 }, 13000, sdlUtils::RIGHT) });
		m_sprites.insert({ "parallax2", std::make_unique<ParallaxSprite>(assetFolder / "bg/start_select.png", this, SDL_FRect{ 0, 198, 256, 198 }, 5000, sdlUtils::RIGHT) });
	}

	void TitleScreen::render() {
		m_sprites.at("topBackground")->draw();
		
		Sprite* gameLogo = m_sprites.at("gameLogo").get();
		if (m_gameLogoOpacityTimer < 500) {
			m_gameLogoOpacityTimer += (SDL_GetTicks() - m_lastTick);
			uint16_t opacity = m_gameLogoOpacityTimer < 0 ? 0 : (m_gameLogoOpacityTimer * 255) / 500;
				
			if (opacity > 255) {
				opacity = 255;
			}
			SDL_SetTextureAlphaMod(gameLogo->m_texture, opacity);
		}
		gameLogo->draw();

		m_sprites.at("parallax1")->draw();
		m_sprites.at("parallax2")->draw();

		m_lastTick = SDL_GetTicks();
	}
};
