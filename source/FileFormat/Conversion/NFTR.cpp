#include "FileFormat/Conversion/NFTR.h"
#include "Utils/stb_image_write.h"

void NFTR::convert(const fileUtils::path& folderPath, const fileUtils::path& outputFolder) {
	convertFileFromFolder(folderPath, outputFolder, ".NFTR", convertToPng);
}

void NFTR::convertToPng(const fileUtils::path& filePath, const fileUtils::path& outputFolder, bool isCompressed) {
	fileUtils::buffer buffer;
	size_t offset = 0;

	if (isCompressed) {
		buffer = fileUtils::decompress(filePath, 4);
	} else {
		buffer = fileUtils::readBin(filePath);
	}

	if (buffer.size() == 0) {
		return;
	}

    const std::vector<Glyph> glyphs = extractNFTR(buffer);
    const fileUtils::buffer glyphWidths = getGlyphWidths(buffer);
    fileUtils::writeBin(glyphWidths, outputFolder / filePath.filename().replace_extension("font"));
    savePNG(glyphs, outputFolder / filePath.filename().replace_extension("png"));
}

fileUtils::buffer NFTR::getGlyphWidths(const fileUtils::buffer& buffer) {
    fileUtils::buffer widths;
    size_t offset = 0x10;

    while (offset < buffer.size()) {
        uint32_t blockType = fileUtils::read4Byte(buffer, offset);
        uint32_t size = fileUtils::read4Byte(buffer, offset);

        if (blockType == 0x43574448) {
            break;
        }

        offset += size - 8;
    }

    if (offset >= buffer.size()) {
        return widths;
    }

    uint16_t firstChar = fileUtils::read2Byte(buffer, offset);
    uint16_t lastChar = fileUtils::read2Byte(buffer, offset);
    uint32_t nextCWDH = fileUtils::read4Byte(buffer, offset);

    for (uint16_t i = firstChar; i <= lastChar; i++) {
        offset += 1;
        uint8_t glyphW = fileUtils::read1Byte(buffer, offset);
        offset += 1;
        widths.push_back(glyphW);
    }

    return widths;
}

std::vector<NFTR::Glyph> NFTR::extractNFTR(const fileUtils::buffer& buffer) {
    size_t offset = 0x10;
    size_t size = 0x10;
    std::vector<Glyph> glyphs;

    while (offset < buffer.size()) {
        uint32_t blockType = fileUtils::read4Byte(buffer, offset);
        size = fileUtils::read4Byte(buffer, offset);

        if (blockType == 0x43474C50) {
            break;
        }

        offset += size - 8;
    }

    if (offset >= buffer.size()) {
        return glyphs;
    }

    size_t blockEnd = offset + size - 8;
    uint8_t cellW = fileUtils::read1Byte(buffer, offset);
    uint8_t cellH = fileUtils::read1Byte(buffer, offset);
    uint16_t tileSize = fileUtils::read2Byte(buffer, offset);
    uint16_t bpp = (8 * tileSize) / cellW / cellH;
    
    offset += 4;

    while (offset + tileSize <= blockEnd) {
        Glyph g;
        g.w = cellW;
        g.h = cellH;
        g.pixels.resize(cellW * cellH);

        for (int i = 0; i < tileSize; i++) {
            uint8_t byte = buffer.at(offset + i);

            if (bpp == 1) {
                int bitCount = 0;
                for (int i = 0; i < tileSize && bitCount < cellW * cellH; i++) {
                    uint8_t byte = buffer.at(offset + i);
                    for (int b = 7; b >= 0 && bitCount < cellW * cellH; b--) {
                        g.pixels[bitCount++] = ((byte >> b) & 1) * 255;
                    }
                }
            } else if (bpp == 2) {
                int base = i * 4;
                g.pixels[base + 0] = ((byte >> 6) & 3) * 85;
                g.pixels[base + 1] = ((byte >> 4) & 3) * 85;
                g.pixels[base + 2] = ((byte >> 2) & 3) * 85;
                g.pixels[base + 3] = (byte & 3) * 85;
            } else if (bpp == 4) {
                int base = i * 2;
                g.pixels[base + 0] = ((byte >> 4) & 0xF) * 17;
                g.pixels[base + 1] = (byte & 0xF) * 17;
            }
        }

        glyphs.push_back(g);
        offset += tileSize;
    }

    return glyphs;
}

void NFTR::savePNG(const std::vector<Glyph>& glyphs, const fileUtils::path& filePath) {
    if (glyphs.empty()) {
        return;
    }

    int cols = 16;
    int w = glyphs.at(0).w;
    int h = glyphs.at(0).h;
    size_t rows = (glyphs.size() + cols - 1) / cols;
    size_t imgW = cols * w;
    size_t imgH = rows * h;
    fileUtils::buffer image(imgW * imgH * 4);

    for (size_t i = 0; i < glyphs.size(); i++) {
        size_t gx = (i % cols) * w;
        size_t gy = (i / cols) * h;

        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                size_t src = y * w + x;
                size_t dst = ((gy + y) * imgW + (gx + x)) * 4;
                uint8_t val = glyphs[i].pixels[src];
                image[dst + 0] = val;
                image[dst + 1] = val;
                image[dst + 2] = val;
                image[dst + 3] = val;
            }
        }
    }

    stbi_write_png(filePath.string().c_str(), static_cast<int>(imgW), static_cast<int>(imgH), 4, image.data(), static_cast<int>(imgW * 4));
}
