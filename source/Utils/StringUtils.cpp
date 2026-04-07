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
		replace(str, "<'e>", "é");
		replace(str, "<^e>", "ê");
		replace(str, "<`e>", "è");
		replace(str, "<e>", "e");
		replace(str, "<`a>", "à");
		replace(str, "<^a>", "â");
		replace(str, "<^o>", "ô");
		replace(str, "<'o>", "ó");
		replace(str, "<^i>", "î");
		replace(str, "<`u>", "ù");
		replace(str, "<^u>", "û");
		replace(str, "<r>", "r");
	}
};
