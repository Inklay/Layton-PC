#pragma once

#include "Game/Sprite/Sprite.h"
#include "FileFormat/Custom/Anim.h"
#include <map>

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture = { -1, -1, -1, -1 });
	void draw(int zIndex = 0) override;
	void unload() override;

protected:
	std::map<int, SDL_Surface*> m_surfaces;
	std::map<int, SDL_Texture*> m_textures;

	Anim m_anim;
	int m_frameIndex;
	int m_frames;
};
