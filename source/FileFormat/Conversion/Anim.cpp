#include "FileFormat/Conversion/Anim.h"

Anim::Anim(const fileUtils::path& inputFile) {
	fileUtils::buffer buffer = fileUtils::readBin(inputFile);
	size_t offset = 0;
	const uint32_t frameCount = fileUtils::read4Byte(buffer, offset);

	m_framesUnk.reserve(frameCount);
	m_imageIdx.reserve(frameCount);

	for (unsigned int i = 0; i < frameCount; i++) {
		m_framesUnk.emplace_back(fileUtils::read4Byte(buffer, offset));
	}

	for (unsigned int i = 0; i < frameCount; i++) {
		m_imageIdx.emplace_back(fileUtils::read4Byte(buffer, offset));
	}
}

void Anim::create(std::vector<uint32_t> framesUnk, std::vector<uint32_t> imageIdx, const fileUtils::path& outputFile) {
	fileUtils::buffer buffer;
	const uint32_t frameCount = (uint32_t)framesUnk.size();

	buffer.reserve((size_t)4 + frameCount * 8);
	fileUtils::write4Byte(buffer, frameCount);

	for (unsigned int i = 0; i < frameCount; i++) {
		fileUtils::write4Byte(buffer, framesUnk.at(i));
	}

	for (unsigned int i = 0; i < frameCount; i++) {
		fileUtils::write4Byte(buffer, imageIdx.at(i));
	}

	fileUtils::writeBin(buffer, outputFile);
}
