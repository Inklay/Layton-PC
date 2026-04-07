#include "FileFormat/Conversion/Anim.h"

Anim::Anim(const fileUtils::path& inputFile) {
	fileUtils::buffer buffer = fileUtils::readBin(inputFile);
	size_t offset = 0;
	const uint32_t frameCount = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);

	offset += 4;
	m_framesUnk.reserve(frameCount);
	m_imageIdx.reserve(frameCount);

	for (unsigned int i = 0; i < frameCount; i++) {
		m_framesUnk.emplace_back((buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset));
		offset += 4;
	}

	for (unsigned int i = 0; i < frameCount; i++) {
		m_imageIdx.emplace_back((buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset));
		offset += 4;
	}
}

void Anim::create(std::vector<uint32_t> framesUnk, std::vector<uint32_t> imageIdx, const fileUtils::path& outputFile) {
	fileUtils::buffer buffer;
	const uint32_t frameCount = framesUnk.size();

	buffer.reserve(4 + frameCount * 8);
	convertByte(buffer, frameCount);

	for (unsigned int i = 0; i < frameCount; i++) {
		convertByte(buffer, framesUnk.at(i));
	}

	for (unsigned int i = 0; i < frameCount; i++) {
		convertByte(buffer, imageIdx.at(i));
	}

	fileUtils::writeBin(buffer, outputFile);
}

void Anim::convertByte(fileUtils::buffer& buffer, const uint32_t byte) {
	buffer.emplace_back((byte & 0xFF));
	buffer.emplace_back((byte & 0xFF00) >> 8);
	buffer.emplace_back((byte & 0xFF0000) >> 16);
	buffer.emplace_back((byte & 0xFF000000) >> 24);
}
