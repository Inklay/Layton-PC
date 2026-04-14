#pragma once

#include "Game/Sprite/CharSprite.h"

class TextSprite : public CharSprite {
public:
	TextSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw(int zIndex = 0) {};
	void drawText(const std::u32string& str) override;
};
