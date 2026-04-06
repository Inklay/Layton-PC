#include <fstream>
#include "FileUtils.h"
#include "FolderUtils.h"
#include <filesystem>

namespace fileUtils {
	buffer readBin(const path& filePath, size_t offset) {
		const size_t file_size = std::filesystem::file_size(filePath) - offset;
		buffer buffer(file_size);
		std::ifstream file(filePath, std::ios_base::binary);

		if (!file.good()) {
			std::cerr << "Can't open file '" << filePath << "'" << std::endl;
			exit(1);
		}

		file.seekg(offset);
		file.read(reinterpret_cast<char*>(buffer.data()), file_size);
		file.close();
		return buffer;
	}

	void writeBin(const buffer& buffer, const path& filePath) {
		folderUtils::createFolderIfDontExists(filePath.parent_path());

		std::ofstream file(filePath, std::ios_base::binary);

		if (!file.good()) {
			std::cerr << "Can't open file '" << filePath << "'" << std::endl;
			exit(1);
		}

		file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		file.close();
	}

	void writeText(const std::string& str, const path& filePath) {
		folderUtils::createFolderIfDontExists(filePath.parent_path());

		std::ofstream file(filePath, std::ios_base::out);

		if (!file.good()) {
			std::cerr << "Can't open file '" << filePath << "'" << std::endl;
			exit(1);
		}
		file.write(str.c_str(), str.length());
		file.close();
	}

	CompressionMethod getCompressionMethod(const path& filePath, size_t offset) {
		std::ifstream file(filePath, std::ios_base::binary);

		if (!file.good()) {
			std::cerr << "Can't open file '" << filePath << "'" << std::endl;
			exit(1);
		}

		file.seekg(offset);
		const unsigned char identificationByte = file.get();
		file.close();
		
		switch (identificationByte) {
			case 0x24:
			case 0x28:
				return CompressionMethod::HUFFMAN;
			case 0x30:
				return CompressionMethod::RLE;
			case 0x10:
			case 0x11:
				return CompressionMethod::LZSS;
			default:
				return CompressionMethod::UNKNOWN;
		}
	}
};
