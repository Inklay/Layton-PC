#pragma once

#include "Utils/FileUtils.h"

class Huffman {
	static fileUtils::buffer decompress(const fileUtils::path& filePath, size_t offset = 0);

	friend fileUtils;
};
