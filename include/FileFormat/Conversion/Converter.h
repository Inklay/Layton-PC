#pragma once

#include "Utils/FileUtils.h"
#include <string>
#include <functional>

class Converter {
protected:
	static void convertFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder, const std::string& extension, std::function<void(const fileUtils::path&, const fileUtils::path&)> callback);
};
