#include <fstream>
#include "Utils/FileUtils.h"
#include "Utils/FolderUtils.h"
#include <filesystem>
#include "FileFormat/Compression/Huffman.h"
#include "FileFormat/Compression/LZSS.h"
#include "FileFormat/Compression/RLE.h"

fileUtils::buffer fileUtils::readBin(const path& filePath, size_t offset, size_t lenght) {
	if (lenght == 0) {
		lenght = std::filesystem::file_size(filePath) - offset;
	}

	buffer buffer(lenght);
	std::ifstream file(filePath, std::ios_base::binary);

	if (!file.good()) {
		std::cerr << "Can't open file '" << filePath << "'" << std::endl;
		exit(1);
	}

	file.seekg(offset);
	file.read(reinterpret_cast<char*>(buffer.data()), lenght);
	file.close();
	return buffer;
}

void fileUtils::writeBin(const buffer& buffer, const path& filePath) {
	folderUtils::createFolderIfDontExists(filePath.parent_path());

	std::ofstream file(filePath, std::ios_base::binary);

	if (!file.good()) {
		std::cerr << "Can't open file '" << filePath << "'" << std::endl;
		exit(1);
	}

	file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	file.close();
}

void fileUtils::writeText(const std::string& str, const path& filePath) {
	folderUtils::createFolderIfDontExists(filePath.parent_path());

	std::ofstream file(filePath, std::ios_base::out);

	if (!file.good()) {
		std::cerr << "Can't open file '" << filePath << "'" << std::endl;
		exit(1);
	}
	file.write(str.c_str(), str.length());
	file.close();
}

fileUtils::buffer fileUtils::decompress(const path& inputFile, size_t offset, CompressionMethod method) {
	if (method == UNKNOWN) {
		method = getCompressionMethod(inputFile, offset);
	}
	switch (method) {
	case HUFFMAN:
		//std::cout << "Huffman compression is not supported yet" << std::endl;
		return Huffman::decompress(inputFile, offset);
		break;
	/*case LZSS:
		return LZSS::decompress(inputFile, offset);*/
	case RLE:
		return RLE::decompress(inputFile, offset);
	default:
		return fileUtils::buffer(0);
	}
}

fileUtils::CompressionMethod fileUtils::getCompressionMethod(const path& inputFile, size_t offset) {
	const buffer readBuffer = readBin(inputFile, offset, 1);
	if (readBuffer.size() == 0) {
		return UNKNOWN;
	}

	switch (readBuffer.at(0)) {
	case 0x24:
	case 0x28:
		return HUFFMAN;
	case 0x30:
		return RLE;
	case 0x10:
	case 0x11:
		return LZSS;
	default:
		return UNKNOWN;
	}
}
