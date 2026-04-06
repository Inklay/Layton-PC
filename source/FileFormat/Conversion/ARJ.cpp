#include "FileFormat/Conversion/ARJ.h"

void ARJ::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".arc", convertToPng);
	convertFileFromFolder(folderPath, outputFolder, ".arj", convertToPng);
}

void ARJ::convertToPng(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	fileUtils::buffer buffer;

	if (isCompressed) {
		buffer = fileUtils::decompress(filePath, 4);
	} else {
		buffer = fileUtils::readBin(filePath);
	}

	if (buffer.size() == 0) {
		return;
	}
}
