#pragma once

#include <string>

namespace stringUtils {
	void addAccents(std::string& str);
	void replace(std::string& str, const std::string& toReplace, const std::string& replaceWith);
	void replace(std::u32string& str, const std::u32string& toReplace, const std::u32string& replaceWith);
	std::u32string toU32(const std::string& utf8);
	std::string toU8(const std::u32string& u32str);
};
