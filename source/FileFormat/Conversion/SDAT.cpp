#include "FileFormat/Conversion/SDAT.h"
#include "FileFormat/Conversion/MID.h"
#include <sstream>

void SDAT::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
    convertFileFromFolder(folderPath, outputFolder, ".sdat", convertToMdi);
}

void SDAT::convertToMdi(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	std::stringstream stream;

#ifdef _WIN32
	const std::string vgmtrans = "bin\\vgmtrans-cli.exe";
	const std::string nullOutput = " > nul 2>&1";
#else
	const std::string vgmtrans = "./bin/vgmtrans-cli";
	const std::string nullOutput = " > /dev/null 2>&1";
#endif

	std::filesystem::create_directory(filePath.parent_path() / "extracted");
	stream << vgmtrans << " " << std::filesystem::absolute(filePath) << " -o " << (filePath.parent_path() / "extracted") << nullOutput;
	system(stream.str().c_str());
	MID::convert(filePath.parent_path() / "extracted", outputFolder);
}
