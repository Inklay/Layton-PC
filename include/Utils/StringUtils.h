#pragma once

#include <string>

namespace stringUtils {
	void addAccents(std::string& str);
	void replace(std::string& str, const std::string& toReplace, const std::string& replaceWith);
};
