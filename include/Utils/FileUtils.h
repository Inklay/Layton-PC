#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

namespace fileUtils {
	enum CompressionMethod {
		LZSS = 1,
		RLE,
		HUFFMAN,
		UNKNOWN
	};

	using buffer = std::vector<unsigned char>;
	using path = std::filesystem::path;

	void writeBin(const buffer& buffer, const path& filePath);
	void writeText(const std::string& str, const path& filePath);
	buffer readBin(const path& filePath, size_t offset = 0);
	CompressionMethod getCompressionMethod(const path& filePath, size_t offset = 0);
};
