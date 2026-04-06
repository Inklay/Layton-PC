#pragma once

#include "FileFormat/Conversion/Converter.h"

class ARJ : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

private:
	static void convertToPng(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
};
