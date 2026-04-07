#include "FileFormat/Conversion/Converter.h"
#include "Utils/FolderUtils.h"

void Converter::convertFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder, const std::string& extension, std::function<void(const fileUtils::path&, const fileUtils::path&, bool)> callback) {
	folderUtils::createFolderIfDontExists(outputFolder);

	for (const auto& item : std::filesystem::directory_iterator(folderPath)) {
		if (item.is_directory()) {
			convertFileFromFolder(item.path(), outputFolder / item.path().stem(), extension, callback);
		} else if (item.is_regular_file() && item.path().extension() == extension) {
			callback(item.path(), outputFolder, isFileCompressed(item.path()));
		}
	}
}

bool Converter::isFileCompressed(const fileUtils::path& file) {
	return file.extension() == ".pcm" || file.extension() == ".arc" || file.extension() == ".arj";
}
