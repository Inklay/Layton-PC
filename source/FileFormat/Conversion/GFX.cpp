#include "FileFormat/Conversion/GFX.h"
#include "FileFormat/Conversion/Anim.h"
#include <cmath>
#include "Utils/stb_image_write.h"
#include "Utils/StringUtils.h"

void GFX::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".arc", convertToPngs);
}

void GFX::convertToPngs(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	fileUtils::buffer buffer = fileUtils::decompress(filePath, 4);
	if (buffer.size() == 0) {
		return;
	}

	unsigned int imageCount = (buffer.at(1) << 8) | buffer.at(0);
	unsigned int colorDepth = ((buffer.at(3) << 8) | buffer.at(2)) == 3 ? 4 : 8;
	unsigned int offset = 4;
	std::vector<GFX::Image> images = getImages(imageCount, colorDepth, buffer, offset);
	if (images.size() == 0) {
		std::cerr << "Invalid GFX file at " << filePath << std::endl;
		return;
	}

	unsigned int paletteCount = ((buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset));
	std::vector<uint16_t> colors;
	colors.reserve(paletteCount);

	for (unsigned int i = 0; i < paletteCount; i++) {
		colors.emplace_back((buffer.at(offset + 5 + i * 2) << 8) | buffer.at(offset + 4 + i * 2));
	}

	offset += 4 + paletteCount * 2 + 0x1E;

	createPng(imageCount, colorDepth, colors, images, filePath, outputFolder);
	if (images.size() > 1) {
		createAnim(buffer, offset, filePath, outputFolder);
	}
}

std::vector<GFX::Image> GFX::getImages(unsigned int imageCount, unsigned int colorDepth, const fileUtils::buffer& buffer, unsigned int& offset) {
	std::vector<GFX::Image> images;
	images.reserve(imageCount);

	for (unsigned int imageIdx = 0; imageIdx < imageCount; imageIdx++) {
		uint16_t width = (buffer.at(offset + 1) << 8) | buffer.at(offset);
		uint16_t height = (buffer.at(offset + 3) << 8) | buffer.at(offset + 2);
		uint16_t partCount = (buffer.at(offset + 5) << 8) | buffer.at(offset + 4);
		std::vector<GFX::Part> parts;

		parts.reserve(partCount);
		offset += 8;

		for (unsigned int partIdx = 0; partIdx < partCount; partIdx++) {
			uint16_t posX = (buffer.at(offset + 1) << 8) | buffer.at(offset);
			uint16_t posY = (buffer.at(offset + 3) << 8) | buffer.at(offset + 2);
			uint16_t width = std::pow(2, ((buffer.at(offset + 5) << 8) | buffer.at(offset + 4)) + 3);
			uint16_t height = std::pow(2, ((buffer.at(offset + 7) << 8) | buffer.at(offset + 6)) + 3);
			uint16_t size = width * height;

			if (colorDepth == 4) {
				size /= 2;
			}

			fileUtils::buffer data;
			data.reserve(size);

			for (unsigned int i = 0; i < size; i++) {
				if ((8 + i + offset) >= buffer.size()) {
					images.clear();
					return images;
				}
				data.emplace_back(buffer.at(8 + i + offset));
			}

			parts.emplace_back(posX, posY, width, height, data);
			offset += 8 + size;
		}

		images.emplace_back(width, height, parts);
	}

	return images;
}

void GFX::createPng(unsigned int imageCount, unsigned int colorDepth, const std::vector<uint16_t>& colors, std::vector<GFX::Image>& images, const fileUtils::path& filePath, const fileUtils::path& outputFolder) {
	for (unsigned int i = 0; i < imageCount; i++) {
		std::pair<uint16_t, uint16_t> size = fixImageSize(images.at(i));
		std::vector<unsigned char> pixelBuffer(size.first * size.second * 4);

		for (GFX::Part& part : images.at(i).m_parts) {
			if (colorDepth == 4) {
				std::vector<uint8_t> newData(part.m_data.size() * 2);

				for (unsigned int j = 0; j < part.m_data.size(); j++) {
					uint8_t color1 = part.m_data.at(j) & 0x0F;
					uint8_t color2 = (part.m_data.at(j) & 0xF0) >> 4;

					newData.at(j * 2) = color1;
					newData.at(j * 2 + 1) = color2;
				}
				part.m_data = newData;
			}

			for (int h = 0; h < part.m_height; h++) {
				for (int w = 0; w < part.m_width; w++) {
					uint16_t colorIdx = part.m_data.at(h * part.m_width + w);

					if (colorIdx == 0) {
						continue;
					}

					uint16_t color = colors[colorIdx];
					int posX = part.m_posX + w;
					int posY = part.m_posY + h;
					size_t pixelOffset = posY * size.first + posX;

					pixelBuffer[pixelOffset * 4 + 0] = (color & 0x001F) * 0x08;
					pixelBuffer[pixelOffset * 4 + 1] = ((color & 0x03E0) >> 5) * 0x08;
					pixelBuffer[pixelOffset * 4 + 2] = ((color & 0x7C00) >> 10) * 0x08;
					pixelBuffer[pixelOffset * 4 + 3] = 255;
				}
			}
		}

		std::string outputFile = (outputFolder / filePath.filename().replace_extension((images.size() > 1 ? std::to_string(i) + "." : "") + "png")).string();
		stringUtils::replace(outputFile, "*", "");
		stringUtils::replace(outputFile, "?", "");
		stbi_write_png(outputFile.c_str(), images.at(i).m_width, images.at(i).m_height, 4, pixelBuffer.data(), size.first * 4);
	}
}

std::pair<uint16_t, uint16_t> GFX::fixImageSize(GFX::Image image) {
	uint16_t width = image.m_width;
	uint16_t height = image.m_height;

	for (const Part& part : image.m_parts) {
		if (part.m_posX + part.m_width > width) {
			width = part.m_posX + part.m_width;
		}

		if (part.m_posY + part.m_height > height) {
			height = part.m_posY + part.m_height;
		}
	}

	return std::make_pair(width, height);
}

void GFX::createAnim(const fileUtils::buffer& buffer, unsigned int offset, const fileUtils::path& filePath, const fileUtils::path& outputFolder) {
	unsigned int animationCount = (buffer.at(offset + 3) << 24) | buffer.at(offset + 2) << 16 | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	std::vector<std::string> animationNames;

	animationNames.reserve(animationCount);
	offset += 4;

	for (unsigned int i = 0; i < animationCount; i++) {
		animationNames.emplace_back("");
		for (unsigned int j = 0; j < 0x1E; j++) {
			char c = buffer.at(offset + j);
			if (c == 0) {

				break;
			}
			animationNames.at(i) += c;
		}
		offset += 0x1E;
	}

	bool skipFirst = animationNames.at(0) == "Create an Animation";

	for (unsigned int i = 0; i < animationCount; i++) {
		uint32_t frameCount = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);

		if (skipFirst && i == 0) {
			offset += 4 + frameCount * 12;
			continue;
		}

		std::vector<uint32_t> framesUnk;
		std::vector<uint32_t> imageIdx;

		framesUnk.reserve(frameCount);
		imageIdx.reserve(frameCount);
		offset += 4 + 4 * frameCount;

		for (unsigned int j = 0; j < frameCount; j++) {
			framesUnk.emplace_back((buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset));
			offset += 4;
		}

		for (unsigned int j = 0; j < frameCount; j++) {
			imageIdx.emplace_back((buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset));
			offset += 4;
		}

		std::string outputFile = filePath.filename().stem().string() + "_" + animationNames.at(i) + ".anim";
		stringUtils::replace(outputFile, "*", "");
		stringUtils::replace(outputFile, "?", "");

		Anim::create(framesUnk, imageIdx, outputFolder / outputFile);
	}
}
