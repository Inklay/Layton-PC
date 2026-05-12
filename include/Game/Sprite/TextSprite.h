#pragma once

#include "Game/Sprite/CharSprite.h"

class TextSprite : public CharSprite {
public:
	TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, SDL_Color color);
	TextSprite(const fileUtils::path& fontFile, const std::u32string& text, Scene* scene, SDL_FRect transform, SDL_Color color);
	TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, SDL_Color color, const std::u32string& toReplace, const std::u32string& replaceWith);
	void draw(int zIndex = 0) override;

private:
	void getFontSize(const fileUtils::path& fontFile);
	SDL_Color m_color;
	float m_width;
	float m_height;
};
