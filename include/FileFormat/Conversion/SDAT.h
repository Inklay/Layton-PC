#pragma once

#include "FileFormat/Conversion/Converter.h"

class SDAT : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

protected:
	static void convertToMdi(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool);
};
