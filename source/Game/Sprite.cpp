#include "Game/Sprite.h"
#include "SDL3_image/SDL_image.h"

Sprite::Sprite(const fileUtils::path& file, SDL_Renderer* renderer, SDL_FRect transform, SDL_FRect subTexture) {
	m_surface = IMG_Load(file.string().c_str());
	m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
	m_transform = transform;
	m_subTexture = subTexture;
}

void Sprite::draw(SDL_Renderer* renderer) const {
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}
