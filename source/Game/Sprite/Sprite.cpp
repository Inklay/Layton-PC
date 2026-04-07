#include "Game/Sprite/Sprite.h"
#include "Game/Scene.h"

Sprite::Sprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) {
	m_surface = IMG_Load(file.string().c_str());
	m_texture = SDL_CreateTextureFromSurface(scene->m_renderer, m_surface);
	m_transform = transform;
	m_subTexture = subTexture;
	m_scene = scene;

	sdlUtils::multiply(m_transform, scene->m_windowMultiplier);
	sdlUtils::multiply(m_subTexture, scene->m_windowMultiplier);
}

void Sprite::draw() {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void Sprite::unload() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroySurface(m_surface);
}
