#include "FileFormat/Conversion/LPCK.h"
#include "Utils/StringUtils.h"
#include <filesystem>

void LPCK::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".pcm", createFilesFromLZSS);
}

void LPCK::createFilesFromLZSS(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	fileUtils::buffer buffer = fileUtils::decompress(filePath, 0, fileUtils::CompressionMethod::LZSS);

	if (buffer.at(m_magicNumberOffset) != 'L' || buffer.at(m_magicNumberOffset + 1) != 'P' || buffer.at(m_magicNumberOffset + 2) != 'C' || buffer.at(m_magicNumberOffset + 3) != 'K') {
		std::cerr << "'" << filePath << "' contains an invalid LPCK file" << std::endl;
		exit(1);
	}

	size_t offset = LPCK::m_headerSize;

	while (offset < buffer.size()) {
		unsigned int fileSize = buffer.at(offset + 7) << 24 | buffer.at(offset + 6) << 16 | buffer.at(offset + 5) << 8 | buffer.at(offset + 4);
		unsigned int dataSize = buffer.at(offset + 15) << 24 | buffer.at(offset + 14) << 16 | buffer.at(offset + 13) << 8 | buffer.at(offset + 12);

		std::string fileName;
		for (int i = 0; i < 16; i++) {
			if (buffer.at(offset + 16 + i) == 0) {
				break;
			}
			fileName.append(1, buffer.at(offset + 16 + i));
		}

		std::string fileData(reinterpret_cast<char*>(&buffer.at(offset + 0x20)), dataSize);
		stringUtils::addAccents(fileData);
		fileUtils::writeText(fileData, outputFolder / fileName);

		offset += fileSize;
	}
}
