#pragma once

#include "FileFormat/Conversion/Converter.h"

class GFX : public Converter {
public:
	static void convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder);

	struct Part {
		Part(uint16_t posX, uint16_t posY, uint64_t width, uint64_t height, fileUtils::buffer data) : m_posX(posX), m_posY(posY), m_width(width), m_height(height), m_data(data) {};
		uint16_t m_posX;
		uint16_t m_posY;
		uint64_t m_width;
		uint64_t m_height;
		fileUtils::buffer m_data;
	};

	struct Image {
		Image(uint16_t width, uint16_t height, std::vector<Part> parts) : m_width(width), m_height(height), m_parts(parts) {};
		uint16_t m_width;
		uint16_t m_height;
		std::vector<Part> m_parts;
	};

private:
	static void convertToPngs(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed);
	static std::vector<Image> getImages(unsigned int imageCount, unsigned int colorDepth, const fileUtils::buffer& buffer, uint64_t& offset, bool isArj);
	static std::pair<uint64_t, uint64_t> fixImageSize(Image image);
	static void createPngs(unsigned int imageCount, unsigned int colorDepth, const std::vector<uint16_t>& colors, std::vector<GFX::Image>& images, const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isArj);
	static void createAnim(const fileUtils::buffer& buffer, uint64_t offset, const fileUtils::path& filePath, const fileUtils::path& outputFolder);
	static void createArcPng(GFX::Part& part, const std::vector<uint16_t>& colors, std::pair<uint64_t, uint64_t> size, fileUtils::buffer& pixelBuffer);
	static void createArjPng(GFX::Part& part, const std::vector<uint16_t>& colors, std::pair<uint64_t, uint64_t> size, fileUtils::buffer& pixelBuffer);

	static const uint8_t bgR = 0x00;
	static const uint8_t bgG = 0xF8;
	static const uint8_t bgB = 0x00;
};
