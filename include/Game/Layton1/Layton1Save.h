#pragma once

#include <Game/Save.h>

class Layton1Save : public Save {
public:
	Layton1Save(fileUtils::path inputFile);
	Layton1Save(const std::u32string name, fileUtils::path file);
	void save();

protected:
	uint16_t m_puzzleCount = 162;
};
