#include "Game/Sprite/Sprite.h"
#include "Game/Scene/Scene.h"
#include "Game/Game.h"

Sprite::Sprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, bool interactive, SDL_FRect subTexture) {
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

Sprite::Sprite(Scene* scene, SDL_FRect transform, SDL_FRect subTexture, bool isInteractive) {
	m_surface = nullptr;
	m_texture = nullptr;
	m_transform = transform;
	m_subTexture = subTexture;
	m_scene = scene;
	m_interactive = isInteractive;

	sdlUtils::multiply(m_transform, scene->m_game->m_windowMultiplier);
	sdlUtils::multiply(m_subTexture, scene->m_game->m_windowMultiplier);
}

void Sprite::draw(int zIndex) {
	m_zIndex = zIndex;
	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	
	computeOpacity();
	SDL_SetTextureAlphaMod(m_texture, m_opacity);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
}

void Sprite::unload() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroySurface(m_surface);
}

void Sprite::setClicked(bool isClicked) {
	m_isClicked = isClicked;
}

bool Sprite::isHovered() {
	float x, y;
	SDL_GetMouseState(&x, &y);

	return x > m_transform.x && x < m_transform.x + m_transform.w && y > m_transform.y && y < m_transform.y + m_transform.h;
}

void Sprite::fade(FadeInfo fadeInfo) {
	m_fadeInfo = fadeInfo;
	m_fading = true;

	if (m_fadeInfo.mode == IN) {
		m_opacity = 0;
	} else {
		m_opacity = 255;
	}
}

void Sprite::computeOpacity() {
	if (m_fadeInfo.timeout < m_fadeInfo.duration) {
		m_fadeInfo.timeout += (int)(SDL_GetTicks() - m_scene->m_lastTick);

		if (m_fadeInfo.mode == IN) {
			m_opacity = (int)(m_fadeInfo.timeout < 0 ? 0 : (m_fadeInfo.timeout * 255) / m_fadeInfo.duration);

			if (m_opacity > 255) {
				m_opacity = 255;
				m_fading = false;
			}
		} else {
			m_opacity = (int)(m_fadeInfo.timeout < 0 ? 255 : (255 - (m_fadeInfo.timeout * 255) / m_fadeInfo.duration));

			if (m_opacity < 0) {
				m_opacity = 0;
				m_fading = false;
			}
		}
	}
}
