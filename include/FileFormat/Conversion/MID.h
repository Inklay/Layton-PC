#pragma once

#include "FileFormat/Conversion/Converter.h"

class MID : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

private:
	static void convertToWav(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
};
