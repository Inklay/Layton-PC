#include "Game/Sprite/ClickableSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

ClickableSprite::ClickableSprite(const fileUtils::path& file, const fileUtils::path& fileClicked, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	Sprite(file, scene, transform, true, subTexture)
{
	m_surfaceClicked = IMG_Load((scene->m_game->m_gameFolder / fileClicked).string().c_str());
	m_textureClicked = SDL_CreateTextureFromSurface(scene->m_game->m_renderer, m_surfaceClicked);

	SDL_SetTextureScaleMode(m_textureClicked, SDL_SCALEMODE_NEAREST);
}

ClickableSprite::ClickableSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	Sprite(file, scene, transform, true, subTexture)
{
	m_surfaceClicked = nullptr;
	m_textureClicked = nullptr;
}

void ClickableSprite::draw(int zIndex) {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_Texture* texture = m_texture;

	m_zIndex = zIndex;

	if (m_isClicked) {
		if (m_textureClicked != nullptr) {
			texture = m_textureClicked;
		} else if (!m_moved) {
			m_transform.x += m_scene->m_game->m_windowMultiplier;
			m_transform.y += m_scene->m_game->m_windowMultiplier;
			m_moved = true;
		}
	}
	
	SDL_RenderTexture(m_scene->m_game->m_renderer, texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void ClickableSprite::setClicked(bool isClicked) {
	m_isClicked = isClicked;

	if (!isClicked) {
		m_moved = false;
		m_transform.x -= m_scene->m_game->m_windowMultiplier;
		m_transform.y -= m_scene->m_game->m_windowMultiplier;
	}
}

void ClickableSprite::unload() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroyTexture(m_textureClicked);
	SDL_DestroySurface(m_surface);
	SDL_DestroySurface(m_surfaceClicked);
}
