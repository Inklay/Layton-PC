#include "LZSS.h"

LZSS::LZSS(const fileUtils::path& filePath, size_t offset) {
#if _DEBUG
	//std::cout << "Decompressing " << filePath << std::endl;
#endif
	m_buffer = fileUtils::readBin(filePath, offset);
	char version = m_buffer.at(0);

	if (m_buffer.size() == 0 || version > versions::LZ11 || version < versions::LZ10) {
		m_version = versions::Unknown;
	}
	else {
		m_version = static_cast<versions>(version);
	}

	m_decompressedSize = 0 | (m_buffer.at(3) << 16) | (m_buffer.at(2) << 8) | m_buffer.at(1);
}

fileUtils::buffer LZSS::decompress() const {
	switch (m_version) {
		case versions::LZ10:
			return decompressLZ10();
		case versions::LZ11:
			return decompressLZ11();
	}
}

fileUtils::buffer LZSS::decompressLZ10() const {
	size_t offset = 3;
	fileUtils::buffer decompressedBuffer;
	decompressedBuffer.reserve(m_decompressedSize);

	while (decompressedBuffer.size() < m_decompressedSize) {
		offset++;
		unsigned int controlByte = m_buffer.at(offset);
		for (int bit = 7; bit >= 0; bit--) {
			if ((controlByte >> bit) & 1) {
				unsigned int a = m_buffer.at(++offset);
				unsigned int b = m_buffer.at(++offset);
				unsigned int sh = (a << 8) | b;
				unsigned int count = (sh >> 0xc) + 3;
				unsigned int disp = (sh & 0xfff) + 1;

				for (unsigned int i = 0; i < count; i++) {
					decompressedBuffer.emplace_back(decompressedBuffer.at(decompressedBuffer.size() - disp));
				}
			}
			else {
				decompressedBuffer.emplace_back(m_buffer.at(++offset));
			}
			if (decompressedBuffer.size() >= m_decompressedSize) {
				break;
			}
		}
	}

	return decompressedBuffer;
}

// TODO
fileUtils::buffer LZSS::decompressLZ11() const {
	fileUtils::buffer buffer(m_decompressedSize);

	return buffer;
}
