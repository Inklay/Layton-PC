#pragma once

#include "FileUtils.h"

namespace LPCK {
	void extractFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);
	void createFilesFromLZSS(const fileUtils::path& filePath, const fileUtils::path& outputFolder);
	static const unsigned int magicNumberOffset = 0xC;
	static const unsigned int headerSize = 0x10;
};
