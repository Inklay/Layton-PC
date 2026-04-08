#include "FileFormat/Conversion/SADL.h"
#include <sstream>

void SADL::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".SAD", convertToWav);
}

void SADL::convertToWav(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	std::stringstream stream;

#ifdef _WIN32
	const std::string vgmstream = "bin\\vgmstream-cli.exe";
	const std::string nullOutput = " > nul 2>&1";
#else
	const std::string vgmstream = "./bin/vgmstream-cli";
	const std::string nullOutput = " > /dev/null 2>&1";
#endif

	stream << vgmstream << " -o" << outputFolder / filePath.filename().replace_extension("wav") << " " << std::filesystem::absolute(filePath) << nullOutput;
	system(stream.str().c_str());
}
