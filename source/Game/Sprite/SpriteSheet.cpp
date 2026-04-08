#include "Game/Sprite/SpriteSheet.h"
#include "Game/Scene.h"
#include "Game/Game.h"

SpriteSheet::SpriteSheet(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	AnimatedSprite(file, scene, transform, subTexture)
{
}

void SpriteSheet::draw() {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_textures.at(m_anim.m_imageIdx.at(m_frameIndex)), subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void SpriteSheet::setIndex(int index) {
	m_frameIndex = index;
}
