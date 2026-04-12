#include "FileFormat/Conversion/MID.h"
#include <sstream>

void MID::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".mid", convertToWav);
}

void MID::convertToWav(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	std::stringstream stream;

#ifdef _WIN32
	const std::string fluidsynth = "bin\\fluidsynth.exe";
	const std::string nullOutput = " > nul 2>&1";
#else
	const std::string fluidsynth = "./bin/fluidsynth";
	const std::string nullOutput = " > /dev/null 2>&1";
#endif

	stream << fluidsynth << " -F " << outputFolder / filePath.filename().replace_extension("wav") << " -g 2 " << filePath << " " << (filePath.parent_path() / filePath.filename().replace_extension("sf2")) << nullOutput;
	system(stream.str().c_str());
}
