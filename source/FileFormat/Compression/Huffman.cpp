#include "FileFormat/Compression/Huffman.h"

fileUtils::buffer Huffman::decompress(const fileUtils::path& filePath, size_t offset) {
	const fileUtils::buffer buffer = fileUtils::readBin(filePath, offset);
	const unsigned int blockSize = buffer.at(0) == 0x24 ? 4 : 8;

	return fileUtils::buffer(0);
}
