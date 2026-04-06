#pragma once

#include "Utils/FileUtils.h"

class LZSS {
	static fileUtils::buffer decompress(const fileUtils::path& filePath, size_t offset = 0);
	static fileUtils::buffer decompressLZ10(size_t decompressedSize, const fileUtils::buffer& buffer);
	static fileUtils::buffer decompressLZ11(size_t decompressedSize, const fileUtils::buffer& buffer);

	friend fileUtils;
};
