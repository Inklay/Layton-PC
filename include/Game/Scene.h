#pragma once

#include "Game/Sprite.h"
#include <map>
#include <string>

class Scene {
public:
	enum Type {
		EXTRACTING_DATA,
		INTRO,
		TITLE_SCREEN
	};

	Scene(Type type);
	virtual void load(const fileUtils::path& assetFolder, SDL_Renderer* renderer, float windowMultiplier) = 0;
	virtual void render() = 0;
	void unload() ;
	Type type() const;

	SDL_Renderer* m_renderer;
	float m_windowMultiplier;

protected:
	Type m_type;
	std::map<std::string, Sprite> m_sprites;
	uint64_t m_lastTick;
};
