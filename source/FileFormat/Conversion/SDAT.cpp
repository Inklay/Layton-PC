#include "FileFormat/Conversion/SDAT.h"

void SDAT::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
    convertFileFromFolder(folderPath, outputFolder, ".sdat", convertToWavs);
}

void SDAT::convertToWavs(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
    
}

void SDAT::writeWav16(const fileUtils::path& filePath, const std::vector<int16_t>& pcm, int sampleRate) {
    fileUtils::buffer buffer;

    int32_t dataSize = static_cast<int32_t>(pcm.size() * 2);
    int32_t fileSize = 36 + dataSize;

    buffer.insert(buffer.end(), { 'R','I','F','F' });
    fileUtils::write4Byte(buffer, fileSize);
    buffer.insert(buffer.end(), { 'W','A','V','E' });
    buffer.insert(buffer.end(), { 'f','m','t',' ' });

    fileUtils::write4Byte(buffer, 16);
    fileUtils::write2Byte(buffer, 1);
    fileUtils::write2Byte(buffer, 1);
    fileUtils::write4Byte(buffer, sampleRate);
    fileUtils::write4Byte(buffer, sampleRate * 2);
    fileUtils::write2Byte(buffer, 2);
    fileUtils::write2Byte(buffer, 16);

    buffer.insert(buffer.end(), { 'd','a','t','a' });
    fileUtils::write4Byte(buffer, dataSize);

    for (size_t i = 0; i < pcm.size(); i++) {
        fileUtils::write2Byte(buffer, static_cast<uint16_t>(pcm[i]));
    }

    fileUtils::writeBin(buffer, filePath);
}
