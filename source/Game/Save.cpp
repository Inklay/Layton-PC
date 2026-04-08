#include "Game/Save.h"

void Save::readCommonData(const fileUtils::buffer& buffer, size_t& offset, GameId id) {
	if (fileUtils::read4Byte(buffer, offset) != m_magicNumber) {
		return;
	}

	uint8_t saveId = fileUtils::read1Byte(buffer, offset);
	if (saveId != id) {
		return;
	}

	m_playTime = fileUtils::read4Byte(buffer, offset);
	m_picarats = fileUtils::read2Byte(buffer, offset);
	uint8_t nameLength = fileUtils::read1Byte(buffer, offset);

	for (size_t i = 0; i < nameLength; i++) {
		m_name += buffer.at(offset + i);
	}

	offset += nameLength;
	m_puzzles.reserve(m_puzzleCount);

	for (size_t i = 0; i < m_puzzleCount; i++) {
		m_puzzles.emplace_back(buffer.at(offset + i));
	}

	offset += nameLength;
	m_id = static_cast<GameId>(saveId);
}
