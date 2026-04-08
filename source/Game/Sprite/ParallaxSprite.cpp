#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

ParallaxSprite::ParallaxSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, int duration, sdlUtils::Dir dir, SDL_FRect subTexture) :
	Sprite(file, scene, transform, false, subTexture)
{
	m_duration = duration;
	m_dir = dir;
	m_transform2 = m_transform;

	switch (m_dir) {
		case sdlUtils::LEFT:
			m_transform2.x += m_transform2.w;
			break;
		case sdlUtils::RIGHT:
			m_transform2.x -= m_transform2.w;
			break;
	}
}

void ParallaxSprite::draw() {
	uint64_t timeElapsed = SDL_GetTicks() - m_scene->m_lastTick;
	uint64_t advancement = static_cast<uint64_t>(((double)timeElapsed / m_duration) * m_transform.w);

	if (m_dir == sdlUtils::LEFT) {
		advancement *= -1;
	}

	if (m_transform.x > m_transform2.x) {
		m_transform.x += advancement;
		m_transform2.x = m_transform.x - m_transform.w;
	} else {
		m_transform2.x += advancement;
		m_transform.x = m_transform2.x - m_transform2.w;
	}

	if (m_dir == sdlUtils::LEFT) {
		if (m_transform.x >= m_transform.w * -1) {
			m_transform.x += m_transform.w * 2;
		}
		if (m_transform2.x >= m_transform2.w * -1) {
			m_transform2.x += m_transform2.w * 2;
		}
	} else if (m_dir == sdlUtils::RIGHT) {
		if (m_transform.x >= m_transform.w) {
			m_transform.x -= m_transform.w * 2;
		}
		if (m_transform2.x >= m_transform2.w) {
			m_transform2.x -= m_transform2.w * 2;
		}
	}

	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform2));
}
