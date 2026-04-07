#pragma once

#include "Game/Sprite/Sprite.h"

class ParallaxSprite : public Sprite {
public:
	ParallaxSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, int duration, sdlUtils::Dir dir, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw() override;

private:
	int m_duration;
	sdlUtils::Dir m_dir;
	SDL_Surface* m_surface2;
	SDL_Texture* m_texture2;
	SDL_FRect m_transform2;
};
