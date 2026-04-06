#include "FileFormat/Compression/RLE.h"

fileUtils::buffer RLE::decompress(const fileUtils::path& filePath, size_t offset) {
	const fileUtils::buffer buffer = fileUtils::readBin(filePath, offset);
	const size_t decompressedSize = 0 | (buffer.at(3) << 16) | (buffer.at(2) << 8) | buffer.at(1);
	size_t bufferOffset = 4;
	size_t decompressed = 0;
	fileUtils::buffer decompressedBuffer;

	decompressedBuffer.reserve(decompressedSize);

	while (decompressed < decompressedSize) {
		unsigned char controlByte = buffer.at(bufferOffset);
		bufferOffset++;
		
		unsigned int count = controlByte & 0x7F;
		if ((controlByte & 0x80) != 0) {
			count += 3;
			unsigned char byte = buffer.at(bufferOffset);
			bufferOffset++;

			for (size_t i = 0; i < count; i++) {
				decompressedBuffer.emplace_back(byte);
				decompressed++;
			}
		} else {
			count ++;
			for (size_t i = 0; i < count; i++) {
				decompressedBuffer.emplace_back(buffer.at(bufferOffset));
				decompressed++;
				bufferOffset++;
			}
		}
	}

	return decompressedBuffer;
}
