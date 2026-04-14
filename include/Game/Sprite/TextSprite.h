#pragma once

#include "Game/Sprite/CharSprite.h"

class TextSprite : public CharSprite {
public:
	TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform);
	TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, const std::u32string& toReplace, const std::u32string& replaceWith);
	void draw(int zIndex = 0) override;
};
