#include "FileFormat/Conversion/MODS.h"
#include <sstream>

void MODS::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".mods", convertToMP4);
}

void MODS::convertToMP4(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	std::stringstream stream;

#ifdef _WIN32
	const std::string ffmpeg = "bin\\ffmpeg.exe";
	const std::string nullOutput = " > nul 2>&1";
#else
	const std::string ffmpeg = "./bin/ffmpeg";
	const std::string nullOutput = " > /dev/null 2>&1";
#endif

	stream << ffmpeg << " -y -i " << std::filesystem::absolute(filePath) << " -vf \"zscale=matrixin=ycgco:matrix=bt709,format=yuv420p\" " << outputFolder / filePath.filename().replace_extension("mp4") << nullOutput;
	system(stream.str().c_str());
}

