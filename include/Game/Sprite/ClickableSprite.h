#pragma once

#include "Game/Sprite/Sprite.h"

class ClickableSprite : public Sprite {
public:
	ClickableSprite(const fileUtils::path& file, const fileUtils::path& fileClicked, Scene* scene, SDL_FRect transform, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw() override;
	void unload() override;
	void setClicked(bool isClicked);

private:
	SDL_Surface* m_surfaceClicked;
	SDL_Texture* m_textureClicked;
	bool m_isClicked = false;
};
