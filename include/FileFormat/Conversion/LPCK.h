#pragma once

#include "FileFormat/Conversion/Converter.h"

class LPCK : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

private:
	static void createFilesFromLZSS(const fileUtils::path& filePath, const fileUtils::path& outputFolder);
	static const unsigned int m_magicNumberOffset = 0xC;
	static const unsigned int m_headerSize = 0x10;
};
