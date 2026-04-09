#include "Utils/StringUtils.h"

namespace stringUtils {
	void replace(std::string& str, const std::string& toReplace, const std::string& replaceWith) {
		size_t pos = str.find(toReplace);

		while (pos != std::string::npos) {
			str.replace(pos, toReplace.length(), replaceWith);
			pos = str.find(toReplace);
		}
	}

	void addAccents(std::string& str) {
		replace(str, "<,c>", "ç");
		replace(str, "<,C>", "Ç");
		replace(str, "<C=>", "€");
		replace(str, "<'e>", "é");
		replace(str, "<^e>", "ê");
		replace(str, "<`e>", "è");
		replace(str, "<:e>", "ë");
		replace(str, "<'E>", "É");
		replace(str, "<`E>", "È");
		replace(str, "<^E>", "Ê");
		replace(str, "<:E>", "Ë");
		replace(str, "<e>" , "ᵉ");
		replace(str, "<`a>", "à");
		replace(str, "<a>",  "ᵃ");
		replace(str, "<'a>", "á");
		replace(str, "<^a>", "â");
		replace(str, "<:a>", "ä");
		replace(str, "<:A>", "Ä");
		replace(str, "<'A>", "Á");
		replace(str, "<`A>", "À");
		replace(str, "<~A>", "Ã");
		replace(str, "<^A>", "Â");
		replace(str, "<'o>", "ó");
		replace(str, "<^o>", "ô");
		replace(str, "<:o>", "ö");
		replace(str, "<`o>", "ò");
		replace(str, "<o>",  "ᵒ");
		replace(str, "<oe>", "œ");
		replace(str, "<:O>", "Ö");
		replace(str, "<^O>", "Ô");
		replace(str, "<OE>", "Œ");
		replace(str, "<^i>", "î");
		replace(str, "<'i>", "í");
		replace(str, "<`i>", "ì");
		replace(str, "<:i>", "ï");
		replace(str, "<`I>", "Ì");
		replace(str, "<'I>", "Í");
		replace(str, "<`u>", "ù");
		replace(str, "<^u>", "û");
		replace(str, "<:u>", "ü");
		replace(str, "<'u>", "ú");
		replace(str, "<:U>", "Ü");
		replace(str, "<'U>", "Ú");
		replace(str, "<`U>", "Ù");
		replace(str, "<^U>", "Û");
		replace(str, "<r>" , "ʳ");
		replace(str, "<ss>", "ß");
		replace(str, "<,,>", "„");
		replace(str, "<66>", "”");
		replace(str, "<99>", "“");
		replace(str, "<~n>", "ñ");
		replace(str, "<~N>", "Ñ");
		replace(str, "<^!>", "¡");
		replace(str, "<^?>", "¿");
		replace(str, "<po>", "£");
		replace(str, "<0>",  "°");
		replace(str, "<2>",  "²");
		replace(str, "<3>",  "³");
		replace(str, "<9>",  "'");
	}
};
