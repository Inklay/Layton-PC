#pragma once

#include "SDL3/SDL.h"
#include "Utils/FileUtils.h"
#include "Utils/SdlUtils.h"
#include "SDL3_image/SDL_image.h"

class Scene;

class Sprite {
public:
	Sprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture = { -1, -1, -1, -1 });
	virtual void draw();
	virtual void unload();

	SDL_Surface* m_surface;
	SDL_Texture* m_texture;
	SDL_FRect m_transform;
	SDL_FRect m_subTexture;
	Scene* m_scene;

protected:
	Sprite(Scene* scene, SDL_FRect transform, SDL_FRect subTexture);
};
