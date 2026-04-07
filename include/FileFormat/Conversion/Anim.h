#pragma once

#include <vector>
#include "Utils/FileUtils.h"

class Anim {
public:
	Anim(const fileUtils::path& inputFile);
	static void create(std::vector<uint32_t> framesUnk, std::vector<uint32_t> imageIdx, const fileUtils::path& outputFile);

	std::vector<uint32_t> m_framesUnk;
	std::vector<uint32_t> m_imageIdx;

private:
	static void convertByte(fileUtils::buffer& buffer, const uint32_t byte);
};