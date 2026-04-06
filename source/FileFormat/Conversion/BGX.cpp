#include "FileFormat/Conversion/BGX.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Utils/stb_image_write.h"

void BGX::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".arc", convertToPng);
	convertFileFromFolder(folderPath, outputFolder, ".bgx", convertToPng);
}

void BGX::convertToPng(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	fileUtils::buffer buffer;

	if (isCompressed) {
		buffer = fileUtils::decompress(filePath, 4);
	} else {
		buffer = fileUtils::readBin(filePath);
	}

	if (buffer.size() == 0) {
		return;
	}

#if _DEBUG
	fileUtils::writeBin(buffer, outputFolder / filePath.filename().replace_extension("bgx"));
#endif

	size_t offset = 0;
	const unsigned int colorPaletteCount = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	std::vector<unsigned int> colorPalettes;
	colorPalettes.reserve(colorPaletteCount);
	offset += 4;

	for (unsigned int i = 0; i < colorPaletteCount; i++) {
		const unsigned int color = (buffer.at(offset + 1) << 8) | buffer.at(offset);
		colorPalettes.emplace_back(color);
		offset += 2;
	}

	const unsigned int tileCount = (buffer.at(offset + 3) << 24) | (buffer.at(offset + 2) << 16) | (buffer.at(offset + 1) << 8) | buffer.at(offset);
	std::vector<std::vector<std::vector<unsigned char>>> tiles;
	tiles.reserve(tileCount);
	offset += 4;

	for (unsigned int tile = 0; tile < tileCount; tile++) {
		std::vector<std::vector<unsigned char>> currentTile(8);

		for (unsigned int row = 0; row < 8; row++) {
			size_t pos = offset + tile * 0x40 + row * 8;
			std::vector<unsigned char> currentRow(8);

			for (unsigned int pixel = 0; pixel < 8; pixel++) {
				currentRow.at(pixel) = buffer.at(pos + pixel);
			}

			currentTile[row] = currentRow;
		}

		tiles.push_back(currentTile);
	}

	offset += 0x40 * tileCount;
	const unsigned int width = (buffer.at(offset + 1) << 8) | buffer.at(offset);
	const unsigned int height = (buffer.at(offset + 3) << 8) | buffer.at(offset + 2);
	offset += 4;
	const unsigned int mapSize = width * height;
	std::vector<unsigned int> map;
	map.reserve(mapSize);

	for (unsigned int count = 0; count < mapSize; count++) {
		const unsigned int tile = (buffer.at(offset + 1 + count * 2) << 8) | buffer.at(offset + count * 2);
		map.emplace_back(tile);
	}

	const unsigned int imgHeight = height * 8;
	const unsigned int imgWidth = width * 8;

	fileUtils::buffer rawImageData(imgHeight * imgWidth * 4);

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			unsigned int tileInfo = map[y * width + x];
			unsigned int tileIdx = tileInfo & 0x3ff;
			bool flipX = (tileInfo >> 11 & 1);
			bool flipY = (tileInfo >> 10 & 1);

			for (unsigned int tileY = 0; tileY < 8; tileY++) {
				unsigned int flippedY = flipY ? 7 - tileY : tileY;

				for (unsigned int tileX = 0; tileX < 8; tileX++) {
					unsigned int flippedX = flipX ? 7 - tileX : tileX;
					unsigned int colorPaletteIdx = tiles[tileIdx][flippedY][flippedX];
					unsigned int color = colorPalettes[colorPaletteIdx];

					unsigned int imgX = x * 8 + tileX;
					unsigned int imgY = y * 8 + tileY;
					unsigned int idx = (imgY * imgWidth + imgX) * 4;

					rawImageData[idx] = (color & 0x1f) * 8;
					rawImageData[idx + 1] = (color >> 5 & 0x1f) * 8;
					rawImageData[idx + 2] = (color >> 10 & 0x1f) * 8;
					rawImageData[idx + 3] = colorPaletteIdx == 0 ? 0x0 : 0xFF;
				}
			}
		}
	}
	fileUtils::path outputFile = outputFolder / filePath.filename().replace_extension("png");
	stbi_write_png(outputFile.string().c_str(), imgWidth, imgHeight, 4, rawImageData.data(), imgWidth * 4);
}
