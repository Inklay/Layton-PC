#include "ARC.h"
#include "LZSS.h"
#include "FolderUtils.h"

namespace ARC {

	void extractFileFromFolder(const fileUtils::path& folderPath, const fileUtils::path& outputFolder, std::function<void(const fileUtils::path&, const fileUtils::buffer&)> callback) {
		for (const auto& item : std::filesystem::directory_iterator(folderPath)) {
			if (item.is_directory()) {
				for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(item.path())) {
					if (!file.is_regular_file() || file.path().extension() != ".arc") {
						continue;
					}

					folderUtils::createFolderIfDontExists(outputFolder / item.path().stem());
					createFilesFromARC(file.path(), outputFolder / item.path().stem(), callback);
				}
			} else if (item.is_regular_file() && item.path().extension() == ".arc") {
				createFilesFromARC(item.path(), outputFolder, callback);
			}
		}
	}

	void createFilesFromARC(const fileUtils::path& filePath, const fileUtils::path& outputFolder, std::function<void(const fileUtils::path&, const fileUtils::buffer&)> callback) {
		const fileUtils::CompressionMethod compressionMethod = fileUtils::getCompressionMethod(filePath, 4);
		fileUtils::buffer buffer;

		switch (compressionMethod) {
			case fileUtils::CompressionMethod::UNKNOWN:
				std::cerr << "Unkown compression method for file '" << filePath << "'" << std::endl;
				exit(1);
			case fileUtils::CompressionMethod::LZSS: {
				const LZSS archive(filePath, 4);
				buffer = archive.decompress();
				break;
			}
			default:
				return;
		}

		callback(outputFolder / filePath.filename(), buffer);
	}
};
