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
	virtual void load(const fileUtils::path& assetFolder, SDL_Renderer* renderer) = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
	void unload() ;
	Type type() const;

protected:
	Type m_type;
	std::map<std::string, Sprite> m_sprites;
	uint64_t m_lastTick;
};
