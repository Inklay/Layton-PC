#pragma once

#include "FileFormat/Conversion/Converter.h"

class MODS : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

private:
	static void convertToMP4(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
};
