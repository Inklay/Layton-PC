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
	uint32_t m_playTime;
	uint16_t m_picarats;
	std::vector<uint8_t> m_puzzles;
	GameId m_id;

protected:
	const uint32_t m_magicNumber = 0x4C554B45; // LUKE
	void readCommonData(const fileUtils::buffer& buffer, size_t& offset, GameId id);
	uint16_t m_puzzleCount;
};
