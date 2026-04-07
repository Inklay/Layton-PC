#pragma once

#include "Game/Sprite/Sprite.h"
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

	uint64_t m_lastTick;
	SDL_Renderer* m_renderer;
	float m_windowMultiplier;

protected:
	Type m_type;
	std::map<std::string, std::unique_ptr<Sprite>> m_sprites;
};
