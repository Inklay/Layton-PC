#pragma once

#include "Game/Sprite/AnimatedSprite.h"

class SpriteSheet : public AnimatedSprite {
public:
	SpriteSheet(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw() override;
	void setIndex(int index);
};
