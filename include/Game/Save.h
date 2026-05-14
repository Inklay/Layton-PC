#pragma once

#include <string>
#include <vector>
#include "Utils/fileUtils.h"

class Save {
public:
	enum GameId {
		UNKNOWN,
		LAYTON1,
		MAX
	};

	virtual void save() = 0;

	std::u32string m_name;
	uint32_t m_playTime = 0;
	uint16_t m_picarats = 0;
	uint16_t m_currentHintCoins = 0;
	uint8_t m_chapter = 0;
	uint8_t m_chapterProgression = 0;
	std::vector<uint8_t> m_puzzles;
	std::vector<uint8_t> m_hintCoins;
	GameId m_id = UNKNOWN;
	int m_solvedPuzzles = 0;

protected:
	const uint32_t m_magicNumber = 0x454B554C; // LUKE
	void readCommonData(const fileUtils::buffer& buffer, size_t& offset);
	void writeCommonData(fileUtils::buffer& buffer);
	uint16_t m_puzzleCount;
	uint16_t m_hintCoinCount;
	fileUtils::path m_filePath;
};
