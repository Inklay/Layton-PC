#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

class fileUtils {
public:
	enum CompressionMethod {
		LZSS = 1,
		RLE,
		HUFFMAN,
		UNKNOWN
	};

	using buffer = std::vector<unsigned char>;
	using path = std::filesystem::path;

	static void writeBin(const buffer& buffer, const path& filePath);
	static void writeText(const std::string& str, const path& filePath);
	static buffer readBin(const path& filePath, size_t offset = 0, size_t lenght = 0);
	static buffer decompress(const path& inputFile, size_t offset = 0, CompressionMethod method = CompressionMethod::UNKNOWN);
	static uint8_t read1Byte(const buffer& buffer, size_t& offset);
	static uint16_t read2Byte(const buffer& buffer, size_t& offset);
	static uint32_t read3Byte(const buffer& buffer, size_t& offset);
	static uint32_t read4Byte(const buffer& buffer, size_t& offset);

private:
	static CompressionMethod getCompressionMethod(const path& inputFile, size_t offset = 0);
};
