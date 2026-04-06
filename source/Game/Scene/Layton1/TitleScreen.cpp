#include "Game/Scene/Layton1/TitleScreen.h"

namespace Layton1Scene {
	void TitleScreen::load(const fileUtils::path& assetFolder, SDL_Renderer* renderer) {
		m_sprites.insert({ "topBackground", Sprite(assetFolder / "bg/select_title.png", renderer, { 0, 0, 512, 384 }) });
	}

	void TitleScreen::render(SDL_Renderer* renderer) {
		m_sprites.at("topBackground").draw(renderer);
	}
};
