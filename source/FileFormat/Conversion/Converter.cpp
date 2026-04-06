#include "FileFormat/Conversion/Converter.h"
#include "Utils/FolderUtils.h"

void Converter::convertFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder, const std::string& extension, std::function<void(const fileUtils::path&, const fileUtils::path&, bool)> callback) {
	for (const auto& item : std::filesystem::directory_iterator(folderPath)) {
		if (item.is_directory()) {
			for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(item.path())) {
				if (!file.is_regular_file() || file.path().extension() != extension) {
					continue;
				}

				folderUtils::createFolderIfDontExists(outputFolder / item.path().stem());
				callback(file.path(), outputFolder / item.path().stem(), isFileCompressed(file.path()));
			}
		}
		else if (item.is_regular_file() && item.path().extension() == extension) {
			callback(item.path(), outputFolder, isFileCompressed(item.path()));
		}
	}
}

bool Converter::isFileCompressed(const fileUtils::path& file) {
	return file.extension() == ".pcm" || file.extension() == ".arc";
}
