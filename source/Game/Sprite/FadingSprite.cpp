#include "Game/Sprite/FadingSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

FadingSprite::FadingSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, int duration, int timer, SDL_FRect subTexture) :
	Sprite(file, scene, transform, false, subTexture)
{
	m_duration = duration;
	m_timer = timer;
}

void FadingSprite::draw(int zIndex) {
	m_zIndex = zIndex;

	if (m_timer < m_duration) {
		m_timer += (int)(SDL_GetTicks() - m_scene->m_lastTick);
		int opacity = (int)(m_timer < 0 ? 0 : (m_timer * 255) / m_duration);

		if (opacity > 255) {
			opacity = 255;
		}
		SDL_SetTextureAlphaMod(m_texture, opacity);
	}

	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}
