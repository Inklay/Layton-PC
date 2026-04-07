#pragma once

#pragma once

#include "Game/Sprite/Sprite.h"

class FadingSprite : public Sprite {
public:
	FadingSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, int duration, int timer = 0, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw() override;

private:
	int m_duration;
	int m_timer;
};
