#include "folderUtils.h"

namespace folderUtils {
	void createFolderIfDontExists(const fileUtils::path& folderPath) {
		if (!std::filesystem::exists(folderPath)) {
			std::filesystem::create_directories(folderPath);
		}
	}
};
