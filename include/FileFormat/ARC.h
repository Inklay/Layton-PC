#pragma once

#include "FileUtils.h"
#include <functional>

namespace ARC {
	void extractFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder, std::function<void(const fileUtils::path&, const fileUtils::buffer&)> callback);
	void createFilesFromARC(const fileUtils::path& filePath, const fileUtils::path& outputFolder, std::function<void(const fileUtils::path&, const fileUtils::buffer&)> callback);
};
