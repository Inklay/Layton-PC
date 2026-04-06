#pragma once

#include <filesystem>
#include <vector>
#include "FileUtils.h"

class LZSS {
public:
	enum versions {
		Unknown = 0x00,
		LZ10 = 0x10,
		LZ11 = 0x11
	};

	LZSS(const fileUtils::path& filePath, size_t offset = 0);
	fileUtils::buffer decompress() const;

private:
	fileUtils::buffer decompressLZ10() const;
	fileUtils::buffer decompressLZ11() const;

	fileUtils::buffer m_buffer;
	versions m_version;
	unsigned int m_decompressedSize;
};
