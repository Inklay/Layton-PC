#pragma once

#include "FileFormat/Conversion/Converter.h"

class NFTR : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

private:
	struct Glyph {
		int w = 0;
		int h = 0;
		fileUtils::buffer pixels;
	};

	static void convertToPng(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
	static std::vector<Glyph> extractNFTR(const fileUtils::buffer& buf);
	static void savePNG(const std::vector<Glyph>& glyphs, const fileUtils::path& filePath);
};
