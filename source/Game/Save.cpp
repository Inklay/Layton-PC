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
	m_hintCoins = fileUtils::read2Byte(buffer, offset);
	uint8_t nameLength = fileUtils::read1Byte(buffer, offset);

	for (size_t i = 0; i < nameLength; i++) {
		m_name += fileUtils::read4Byte(buffer, offset);
	}

	m_puzzles.reserve(m_puzzleCount);

	for (size_t i = 0; i < m_puzzleCount; i++) {
		m_puzzles.emplace_back(buffer.at(offset + i));
	}

	offset += nameLength;
	m_id = static_cast<GameId>(saveId);
}

void Save::writeCommonData(fileUtils::buffer& buffer) {
	fileUtils::write4Byte(buffer, m_magicNumber);
	fileUtils::write1Byte(buffer, LAYTON1);
	fileUtils::write4Byte(buffer, m_playTime);
	fileUtils::write2Byte(buffer, m_picarats);
	fileUtils::write2Byte(buffer, m_hintCoins);
	fileUtils::write1Byte(buffer, (uint8_t)m_name.length());

	for (uint8_t i = 0; i < m_name.length(); i++) {
		fileUtils::write4Byte(buffer, m_name.at(i));
	}

	for (size_t i = 0; i < m_puzzles.size(); i++) {
		fileUtils::write1Byte(buffer, m_puzzles.at(i));
	}
}
