#include "FileFormat/Conversion/SADL.h"

void SADL::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".sad", convertToWav);
}

void SADL::convertToWav(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {

}
