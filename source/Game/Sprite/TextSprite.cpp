#include "Game/Sprite/TextSprite.h"
#include "Game/Scene/Scene.h"
#include "Game/Game.h"
#include "Utils/StringUtils.h"
#include <algorithm>

TextSprite::TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, SDL_Color color) :
	CharSprite(fontFile, scene, transform),
	m_color(color)
{
	m_str = fileUtils::readText(scene->m_game->m_gameFolder / textFile);
	getFontSize(fontFile);
}

TextSprite::TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, SDL_Color color, const std::u32string& toReplace, const std::u32string& replaceWith) :
	CharSprite(fontFile, scene, transform),
	m_color(color)
{
	m_str = fileUtils::readText(scene->m_game->m_gameFolder / textFile);
	getFontSize(fontFile);

	if (!toReplace.empty()) {
		stringUtils::replace(m_str, toReplace, replaceWith);
	}
}

TextSprite::TextSprite(const fileUtils::path& fontFile, const std::u32string& text, Scene* scene, SDL_FRect transform, SDL_Color color) :
	CharSprite(fontFile, scene, transform),
	m_color(color)
{
	m_str = text;
	getFontSize(fontFile);
}

void TextSprite::getFontSize(const fileUtils::path& fontFile) {
	if (fontFile == "font/fontevent.png") {
		m_width = 9.0f;
		m_height = 12.0f;
	} else if (fontFile == "font/fontq.png") {
		m_width = 7.0f;
		m_height = 10.0f;
	}
}

void TextSprite::draw(int) {
	const int colSize = 16;

	if (m_transform.x == -1 * m_scene->m_game->m_windowMultiplier) {
		m_transform.x = WIDTH / 2;

		for (size_t i = 0; i < m_str.length(); i++) {
			const std::vector<std::u32string>::iterator charIt = std::find(m_fontChars.begin(), m_fontChars.end(), std::u32string(1, m_str.at(i)));

			if (charIt == m_fontChars.end()) {
				return;
			}

			const uint64_t charIdx = charIt - m_fontChars.begin();
			m_transform.x -= (m_glyphWidths.at(charIdx) + 2) / 2;
		}

		m_transform.x *= m_scene->m_game->m_windowMultiplier;
	}

	if (m_transform.y == -1 * m_scene->m_game->m_windowMultiplier) {
		m_transform.y = (HALF_HEIGHT * 1.5f - m_height) * m_scene->m_game->m_windowMultiplier;
	}

	if (m_translating) {
		computeTranslation();
	}

	const SDL_FRect baseRect = m_transform;
	m_transform.h = m_height * m_scene->m_game->m_windowMultiplier;
	m_transform.w = m_width * m_scene->m_game->m_windowMultiplier;

	for (size_t i = 0; i < m_str.length(); i++) {
		if (m_str.at(i) == U'\n') {
			m_transform.y += 14 * m_scene->m_game->m_windowMultiplier;
			m_transform.x = baseRect.x;
			continue;
		}

		const std::vector<std::u32string>::iterator charIt = std::find(m_fontChars.begin(), m_fontChars.end(), std::u32string(1, m_str.at(i)));
	
		if (charIt == m_fontChars.end()) {
			return;
		}

		const uint64_t charIdx = charIt - m_fontChars.begin();
		const float x = static_cast<float>(charIdx % colSize * m_width);
		const float y = static_cast<float>(charIdx / colSize * m_height);
		
		const SDL_FRect subTexture{ x, y, m_width, m_height };
		SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);
		SDL_SetTextureAlphaMod(m_texture, m_opacity);
		SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, &subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));

		float glyphWidth = (float)m_glyphWidths.at(charIdx);

		if (glyphWidth == 0) {
			glyphWidth = 2;
		}

		m_transform.x += (glyphWidth + 1) * m_scene->m_game->m_windowMultiplier;
	}
	m_transform = baseRect;
}
