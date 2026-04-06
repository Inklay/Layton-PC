#include "FileFormat/Compression/LZSS.h"

fileUtils::buffer LZSS::decompress(const fileUtils::path& filePath, size_t offset) {
	fileUtils::buffer buffer = fileUtils::readBin(filePath, offset);

	char versionByte = buffer.at(0);
	size_t decompressedSize = 0 | (buffer.at(3) << 16) | (buffer.at(2) << 8) | buffer.at(1);

	switch (versionByte) {
		case 0x10:
			return decompressLZ10(decompressedSize, buffer);
		case 0x11:
			return decompressLZ11(decompressedSize, buffer);
	}
}

fileUtils::buffer LZSS::decompressLZ10(size_t decompressedSize, const fileUtils::buffer& buffer) {
	size_t offset = 3;
	fileUtils::buffer decompressedBuffer;
	decompressedBuffer.reserve(decompressedSize);

	while (decompressedBuffer.size() < decompressedSize) {
		offset++;
		unsigned int controlByte = buffer.at(offset);
		for (int bit = 7; bit >= 0; bit--) {
			if ((controlByte >> bit) & 1) {
				unsigned int a = buffer.at(++offset);
				unsigned int b = buffer.at(++offset);
				unsigned int sh = (a << 8) | b;
				unsigned int count = (sh >> 0xc) + 3;
				unsigned int disp = (sh & 0xfff) + 1;

				for (unsigned int i = 0; i < count; i++) {
					decompressedBuffer.emplace_back(decompressedBuffer.at(decompressedBuffer.size() - disp));
				}
			} else {
				decompressedBuffer.emplace_back(buffer.at(++offset));
			}

			if (decompressedBuffer.size() >= decompressedSize) {
				break;
			}
		}
	}

	return decompressedBuffer;
}

// TODO
fileUtils::buffer LZSS::decompressLZ11(size_t decompressedSize, const fileUtils::buffer& buffer) {
	fileUtils::buffer decompressedBuffer;
	decompressedBuffer.reserve(decompressedSize);

	return decompressedBuffer;
}
