#include "LPCK.h"
#include "LZSS.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include <filesystem>


namespace LPCK {

void extractFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	for (const auto& languageFolder : std::filesystem::directory_iterator(folderPath)) {
		if (!languageFolder.is_directory()) {
			continue;
		}

		for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(languageFolder.path())) {
			if (!file.is_regular_file() || file.path().extension() != ".pcm") {
				continue;
			}

			createFilesFromLZSS(file.path(), outputFolder / languageFolder.path().stem());
		}
	}
}

void createFilesFromLZSS(const fileUtils::path& filePath, const fileUtils::path& outputFolder) {
	LZSS archive = LZSS(filePath);
	fileUtils::buffer buffer = archive.decompress();

#if _DEBUG
	fileUtils::writeBin(buffer, outputFolder / filePath.filename().replace_extension("lpck"));
#endif

	if (buffer.at(LPCK::magicNumberOffset) != 'L' || buffer.at(LPCK::magicNumberOffset + 1) != 'P' || buffer.at(LPCK::magicNumberOffset + 2) != 'C' || buffer.at(LPCK::magicNumberOffset + 3) != 'K') {
		std::cerr << "'" << filePath << "' contains an invalid LPCK file" << std::endl;
		exit(1);
	}

	size_t offset = LPCK::headerSize;

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

};
