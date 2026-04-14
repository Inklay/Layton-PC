#pragma once

#include "Game/Sprite/Sprite.h"

class CharSprite : public Sprite {
public:
	CharSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform);
	void draw(int zIndex = 0) {};
	void drawText(const std::u32string& c) override;

protected:
	std::vector<std::u32string> m_fontChars = {
		U" ", U"!", U"\"", U"#", U"$", U"%", U"&", U"'", U"(", U")", U"*",  U"+", U",", U"-", U".", U"/",
		U"0", U"1",  U"2", U"3", U"4", U"5", U"6", U"7", U"8", U"9", U":",  U";", U"=", U"?", U"@", U"A",
		U"B", U"C",  U"D", U"E", U"F", U"G", U"H", U"I", U"J", U"K", U"L",  U"M", U"N", U"O", U"P", U"Q",
		U"R", U"S",  U"T", U"U", U"V", U"W", U"X", U"Y", U"Z", U"[", U"\\", U"]", U"^", U"_", U"a", U"b",
		U"c", U"d",  U"e", U"f", U"g", U"h", U"i", U"j", U"k", U"l", U"m",  U"n", U"o", U"p", U"q", U"r",
		U"s", U"t",  U"u", U"v", U"w", U"x", U"y", U"z", U"€", U"‚", U"„",  U"<", U"Œ", U"‘", U"’", U"“",
		U"”", U">",  U"œ", U"¡", U"£", U"ᵃ", U"«", U"ᵉ", U"ʳ", U"²", U"³",  U"°", U"ᵒ", U"»", U"¿", U"À",
		U"Á", U"Â",  U"Ã", U"Ä", U"Å", U"Ç", U"È", U"É", U"Ê", U"Ë", U"Ì",  U"Í", U"Î", U"Ï", U"Ñ", U"Ò",
		U"Ó", U"Ô",  U"Ö", U"Ù", U"Ú", U"Û", U"Ü", U"ß", U"à", U"á", U"â",  U"ä", U"ç", U"è", U"é", U"ê",
		U"ë", U"ì",  U"í", U"î", U"ñ", U"ò", U"ó", U"ô", U"ö", U"ù", U"ú",  U"û", U"ü"
	};
	fileUtils::buffer m_glyphWidths;
	std::u32string m_str;
};
