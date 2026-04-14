#include "Game/Sprite/TextSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include "Utils/StringUtils.h"
#include <algorithm>

TextSprite::TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform) :
	CharSprite(fontFile, scene, transform)
{
	m_str = fileUtils::readText(scene->m_game->m_gameFolder / textFile);
}

TextSprite::TextSprite(const fileUtils::path& fontFile, const fileUtils::path& textFile, Scene* scene, SDL_FRect transform, const std::u32string& toReplace, const std::u32string& replaceWith) :
	CharSprite(fontFile, scene, transform)
{
	m_str = fileUtils::readText(scene->m_game->m_gameFolder / textFile);

	if (!toReplace.empty()) {
		stringUtils::replace(m_str, toReplace, replaceWith);
	}
}

void TextSprite::draw(int) {
	const float height = 12.0f;
	const float width = 9.0f;
	const int colSize = 16;
	const SDL_FRect baseRect = m_transform;
	m_transform.h = height * m_scene->m_game->m_windowMultiplier;
	m_transform.w = width * m_scene->m_game->m_windowMultiplier;

	for (size_t i = 0; i < m_str.length(); i++) {
		const std::vector<std::u32string>::iterator charIt = std::find(m_fontChars.begin(), m_fontChars.end(), std::u32string(1, m_str.at(i)));

		if (charIt == m_fontChars.end()) {
			return;
		}

		const uint64_t charIdx = charIt - m_fontChars.begin();
		const float x = static_cast<float>(charIdx % colSize * width);
		const float y = static_cast<float>(charIdx / colSize * height);
		
		const SDL_FRect subTexture{ x, y, width, height };
		SDL_SetTextureColorMod(m_texture, 0, 0, 0);
		SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, &subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));

		float glyphWidth = (float)m_glyphWidths.at(charIdx);

		if (glyphWidth == 0) {
			glyphWidth = 2;
		}

		m_transform.x += (glyphWidth + 1) * m_scene->m_game->m_windowMultiplier;
	}
	m_transform = baseRect;
}
