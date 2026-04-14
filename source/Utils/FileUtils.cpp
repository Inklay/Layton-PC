#include <fstream>
#include "Utils/FileUtils.h"
#include "Utils/FolderUtils.h"
#include "Utils/StringUtils.h"
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

std::u32string fileUtils::readText(const path& filePath) {
	size_t lenght = std::filesystem::file_size(filePath);
	std::string u8str;
	std::u32string str;
	std::ifstream file(filePath, std::ios_base::in);

	if (!file.good()) {
		std::cerr << "Can't open file '" << filePath << "'" << std::endl;
		exit(1);
	}

	file.read(u8str.data(), lenght);
	return stringUtils::toU32(u8str);
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
			return Huffman::decompress(inputFile, offset);
		case LZSS:
			return LZSS::decompress(inputFile, offset);
		case RLE:
			return RLE::decompress(inputFile, offset);
		default:
			std::cerr << "Unknown compression method for " << inputFile << std::endl;
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

void fileUtils::write1Byte(buffer& buffer, uint8_t data) {
	buffer.emplace_back((uint8_t)(data & 0xFF));
}

void fileUtils::write2Byte(buffer& buffer, uint16_t data) {
	buffer.emplace_back((uint8_t)(data & 0xFF));
	buffer.emplace_back((uint8_t)((data & 0xFF00) >> 8));
}

void fileUtils::write3Byte(buffer& buffer, uint32_t data) {
	buffer.emplace_back((uint8_t)(data & 0xFF));
	buffer.emplace_back((uint8_t)((data & 0xFF00) >> 8));
	buffer.emplace_back((uint8_t)((data & 0xFF0000) >> 16));
}

void fileUtils::write4Byte(buffer& buffer, uint32_t data) {
	buffer.emplace_back((uint8_t)(data & 0xFF));
	buffer.emplace_back((uint8_t)((data & 0xFF00) >> 8));
	buffer.emplace_back((uint8_t)((data & 0xFF0000) >> 16));
	buffer.emplace_back((uint8_t)((data & 0xFF000000) >> 24));
}

uint8_t fileUtils::read1Byte(const buffer& buffer, size_t& offset) {
	uint8_t data = buffer.at(offset);
	offset++;
	return data;
}

uint16_t fileUtils::read2Byte(const buffer& buffer, size_t& offset) {
	uint16_t data = (buffer.at(offset + 1) << 8) | buffer.at(offset);
	offset += 2;
	return data;
}

uint32_t fileUtils::read3Byte(const buffer& buffer, size_t& offset) {
	uint32_t data = (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	offset += 3;
	return data;
}

uint32_t fileUtils::read4Byte(const buffer& buffer, size_t& offset) {
	uint32_t data = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	offset += 4;
	return data;
}

uint8_t fileUtils::read1Byte(const buffer& buffer, const size_t&& offset) {
	uint8_t data = buffer.at(offset);
	return data;
}

uint16_t fileUtils::read2Byte(const buffer& buffer, const size_t&& offset) {
	uint16_t data = (buffer.at(offset + 1) << 8) | buffer.at(offset);
	return data;
}

uint32_t fileUtils::read3Byte(const buffer& buffer, const size_t&& offset) {
	uint32_t data = (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	return data;
}

uint32_t fileUtils::read4Byte(const buffer& buffer, const size_t&& offset) {
	uint32_t data = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	return data;
}

std::string fileUtils::readNullTerminatedString(const buffer& buffer, size_t& offset, int maxLenght) {
	int count = 0;
	uint8_t c = buffer.at(offset);
	std::string string;

	while (c != 0) {
		string += c;
		c = buffer.at(offset);

		if (string.length() == maxLenght) {
			break;
		}

		offset++;
	}

	return string;
}

std::string fileUtils::readNullTerminatedString(const buffer& buffer, const size_t&& offset, int maxLenght) {
	int count = 0;
	uint8_t c = buffer.at(offset);
	std::string string;
	size_t stringOffset = offset;

	while (c != 0) {
		string += c;
		c = buffer.at(stringOffset);

		if (string.length() == maxLenght) {
			break;
		}

		stringOffset++;
	}

	return string;
}
