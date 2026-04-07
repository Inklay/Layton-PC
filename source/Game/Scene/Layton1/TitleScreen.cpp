#include "Game/Scene/Layton1/TitleScreen.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, float windowMultiplier) {
		m_gameLogoOpacityTimer = -1500;
		m_lastTick = 0;
		m_renderer = renderer;
		m_windowMultiplier = windowMultiplier;

		m_sprites.insert({ "topBackground", Sprite(assetFolder / "bg/select_title.png", this, { 0, 0, 512, 384 }) });
		m_sprites.insert({ "gameLogo", Sprite(assetFolder / "ani/fr/title_logo.png", this, { 86, 116, 340, 148 }) });
	}

	void TitleScreen::render() {
		m_sprites.at("topBackground").draw(m_renderer);

		const Sprite& gameLogo = m_sprites.at("gameLogo");
		if (m_gameLogoOpacityTimer < 500) {
			m_gameLogoOpacityTimer += (SDL_GetTicks() - m_lastTick);
			uint16_t opacity = m_gameLogoOpacityTimer < 0 ? 0 : (m_gameLogoOpacityTimer * 255) / 500;
				
			if (opacity > 255) {
				opacity = 255;
			}
			SDL_SetTextureAlphaMod(gameLogo.m_texture, opacity);
		}
		gameLogo.draw(m_renderer);

		m_lastTick = SDL_GetTicks();
	}
};
