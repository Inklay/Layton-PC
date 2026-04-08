#include "Game/Sprite/Sprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

Sprite::Sprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture, bool interactive) {
	m_scene = scene;
	m_surface = IMG_Load((m_scene->m_game->m_gameFolder / file).string().c_str());
	m_texture = SDL_CreateTextureFromSurface(scene->m_game->m_renderer, m_surface);
	m_transform = transform;
	m_subTexture = subTexture;
	m_interactive = interactive;

	SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);

	sdlUtils::multiply(m_transform, scene->m_game->m_windowMultiplier);
	sdlUtils::multiply(m_subTexture, scene->m_game->m_windowMultiplier);
}

Sprite::Sprite(Scene* scene, SDL_FRect transform, SDL_FRect subTexture) {
	m_surface = nullptr;
	m_texture = nullptr;
	m_transform = transform;
	m_subTexture = subTexture;
	m_scene = scene;
	m_interactive = false;

	sdlUtils::multiply(m_transform, scene->m_game->m_windowMultiplier);
	sdlUtils::multiply(m_subTexture, scene->m_game->m_windowMultiplier);
}

void Sprite::draw() {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void Sprite::unload() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroySurface(m_surface);
}
