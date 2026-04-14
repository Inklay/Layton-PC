#include "Game/Sprite/TextSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include <algorithm>

TextSprite::TextSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	CharSprite(file, scene, transform, subTexture)
{
}

void TextSprite::drawText(const std::u32string& str) {
	const float height = 12.0f;
	const float width = 9.0f;
	const int colSize = 16;
	const float baseX = m_transform.x;

	for (size_t i = 0; i < str.length(); i++) {
		const std::vector<std::u32string>::iterator charIt = std::find(m_fontChars.begin(), m_fontChars.end(), str);

		if (charIt == m_fontChars.end()) {
			return;
		}

		const uint64_t charIdx = charIt - m_fontChars.begin();
		const float x = static_cast<float>(charIdx % colSize * width);
		const float y = static_cast<float>(charIdx / colSize * height);
		
		m_transform.x += (width - m_glyphWidths.at(charIdx));

		const SDL_FRect subTexture{ x, y, width, height };
		SDL_SetTextureColorMod(m_texture, 0, 0, 0);
		SDL_RenderTexture(m_scene->m_game->m_renderer, m_texture, &subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));
	}
	m_transform.x = baseX;
}
