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

	if (m_shaking) {
		m_shakeInfo.duration -= (int)(SDL_GetTicks() - m_scene->m_lastTick);
		m_shakeInfo.timer++;

		if (m_shakeInfo.duration <= 0) {
			m_shakeInfo.duration = 0;
			m_shakeInfo.timer = 0;
			m_shaking = false;

			m_transform.x -= m_shakeInfo.offset * m_scene->m_game->m_windowMultiplier;
			m_transform.y -= m_shakeInfo.offset * m_scene->m_game->m_windowMultiplier;
		} else if (m_shakeInfo.timer == 5) {
			m_shakeInfo.timer = 0;
			m_shakeInfo.offset = m_shakeInfo.offset > 0 ? -1 : 1;

			m_transform.x += m_shakeInfo.offset * m_scene->m_game->m_windowMultiplier;
			m_transform.y += m_shakeInfo.offset * m_scene->m_game->m_windowMultiplier;
		}
	}

	if (m_translating) {
		computeTranslation();
	}

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

			if (m_opacity >= 255) {
				m_opacity = 255;
				m_fading = false;
			}
		} else {
			m_opacity = (int)(m_fadeInfo.timeout < 0 ? 255 : (255 - (m_fadeInfo.timeout * 255) / m_fadeInfo.duration));

			if (m_opacity <= 0) {
				m_opacity = 0;
				m_fading = false;
			}
		}
	}
}

void Sprite::computeTranslation() {
	int elapsed = (int)(SDL_GetTicks() - m_scene->m_lastTick);

	m_transform.x += m_translationInfo.x * ((float)elapsed / m_translationInfo.duration) * m_scene->m_game->m_windowMultiplier;
	m_transform.y += m_translationInfo.y * ((float)elapsed / m_translationInfo.duration) * m_scene->m_game->m_windowMultiplier;
	m_translationInfo.elapsed += elapsed;

	if (m_translationInfo.elapsed > m_translationInfo.duration) {
		m_transform.x = m_translationInfo.newX;
		m_transform.y = m_translationInfo.newY;
		m_translationInfo.elapsed = 0;
		m_translating = false;
	}
}

void Sprite::shake(int duration) {
	m_shaking = true;
	m_shakeInfo.duration = duration;
}

void Sprite::translate(TranslationInfo translationInfo) {
	m_translating = true;
	m_translationInfo = translationInfo;
	m_translationInfo.newX = m_transform.x + m_translationInfo.x * m_scene->m_game->m_windowMultiplier;
	m_translationInfo.newY = m_transform.y + m_translationInfo.y * m_scene->m_game->m_windowMultiplier;
}
