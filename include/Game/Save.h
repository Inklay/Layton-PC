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

	std::string m_name;
	uint32_t m_playTime = 0;
	uint16_t m_picarats = 0;
	std::vector<uint8_t> m_puzzles;
	GameId m_id = UNKNOWN;

protected:
	const uint32_t m_magicNumber = 0x454B554C; // LUKE
	void readCommonData(const fileUtils::buffer& buffer, size_t& offset, GameId id);
	uint16_t m_puzzleCount = 0;
	fileUtils::path m_filePath;
};
