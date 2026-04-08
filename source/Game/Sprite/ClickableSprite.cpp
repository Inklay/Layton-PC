#include "Game/Sprite/ClickableSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

ClickableSprite::ClickableSprite(const fileUtils::path& file, const fileUtils::path& fileClicked, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	Sprite(file, scene, transform, subTexture, true)
{
	m_surfaceClicked = IMG_Load((scene->m_game->m_gameFolder / fileClicked).string().c_str());
	m_textureClicked = SDL_CreateTextureFromSurface(scene->m_game->m_renderer, m_surface);
}

void ClickableSprite::draw() {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_Texture* texture = m_isClicked ? m_textureClicked : m_texture;
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void ClickableSprite::setClicked(bool isClicked) {
	m_isClicked = isClicked;
}

void ClickableSprite::unload() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroyTexture(m_textureClicked);
	SDL_DestroySurface(m_surface);
	SDL_DestroySurface(m_surfaceClicked);
}
