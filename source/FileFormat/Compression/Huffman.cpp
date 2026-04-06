#include "FileFormat/Compression/Huffman.h"

fileUtils::buffer Huffman::decompress(const fileUtils::path& filePath, size_t offset) {
	fileUtils::buffer buffer = fileUtils::readBin(filePath, offset);

	return buffer;
}
