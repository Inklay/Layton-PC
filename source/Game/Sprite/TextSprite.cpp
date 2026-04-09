#include "Game/Sprite/TextSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

TextSprite::TextSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	Sprite(file, scene, transform, false, subTexture)
{
}

void TextSprite::drawText(char c) {
	SDL_SetTextureColorMod(m_texture, 0, 0, 0);
	const SDL_FRect subTexture{ 9, 0, 9, 12 };
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, &subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}
