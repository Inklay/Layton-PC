#pragma once

#include "Game/Sprite/Sprite.h"

class FadingSprite : public Sprite {
public:
	FadingSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, int duration, int timer = 0, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw(int zIndex = 0) override;

private:
	size_t m_duration;
	size_t m_timer;
};
