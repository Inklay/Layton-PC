#pragma once

#include "FileFormat/Conversion/Converter.h"

class SDAT : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

protected:
	static void convertToWavs(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
	static void writeWav16(const fileUtils::path& filePath, const std::vector<int16_t>& pcm, int sampleRate = 16000);
};
