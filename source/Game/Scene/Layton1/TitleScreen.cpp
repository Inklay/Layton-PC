#include "Game/Scene/Layton1/TitleScreen.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer) {
		m_sprites.insert({ "topBackground", Sprite(assetFolder / "bg/select_title.png", renderer, { 0, 0, 512, 384 }) });
		m_sprites.insert({ "gameLogo", Sprite(assetFolder / "ani/fr/title_logo.0.png", renderer, { 86, 116, 340, 148 }) });

		m_gameLogoOpacityTimer = -1500;
		m_lastTick = 0;
	}

	void TitleScreen::render(SDL_Renderer* renderer) {
		m_sprites.at("topBackground").draw(renderer);

		const Sprite& gameLogo = m_sprites.at("gameLogo");
		if (m_gameLogoOpacityTimer < 500) {
			m_gameLogoOpacityTimer += (SDL_GetTicks() - m_lastTick);
			uint16_t opacity = m_gameLogoOpacityTimer < 0 ? 0 : (m_gameLogoOpacityTimer * 255) / 500;
				
			std::cout << (int)opacity << std::endl;
			if (opacity > 255) {
				opacity = 255;
			}
			SDL_SetTextureAlphaMod(gameLogo.m_texture, opacity);
		}
		gameLogo.draw(renderer);

		m_lastTick = SDL_GetTicks();
	}
};
