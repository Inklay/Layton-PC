#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils/FileUtils.h"
#include "Utils/SdlUtils.h"

class Scene;

class Sprite {
public:
	enum FadingMode {
		IN,
		OUT
	};

	struct FadeInfo {
		int duration;
		int timeout;
		FadingMode mode;
	};

	Sprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, bool isInteractive = false, SDL_FRect subTexture = { -1, -1, -1, -1 });
	virtual void draw(int zIndex = 0);
	virtual void unload();
	virtual void setText(const std::u32string& str) {};
	virtual void fade(FadeInfo fadeInfo);

	SDL_Surface* m_surface;
	SDL_Texture* m_texture;
	SDL_FRect m_transform;
	SDL_FRect m_subTexture;
	Scene* m_scene;
	virtual void setClicked(bool isClicked);
	bool isHovered();
	int m_zIndex = 0;
	int m_opacity = 255;
	FadeInfo m_fadeInfo;
	bool m_interactive;
	bool m_fading = false;

protected:
	Sprite(Scene* scene, SDL_FRect transform, SDL_FRect subTexture, bool isInteractive = false);
	bool m_isClicked = false;
	void computeOpacity();
};
