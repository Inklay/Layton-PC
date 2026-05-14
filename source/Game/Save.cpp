#include "Game/Save.h"

void Save::readCommonData(const fileUtils::buffer& buffer, size_t& offset) {
	if (fileUtils::read4Byte(buffer, offset) != m_magicNumber) {
		return;
	}

	uint8_t saveId = fileUtils::read1Byte(buffer, offset);
	if (saveId != m_id) {
		return;
	}

	m_playTime = fileUtils::read4Byte(buffer, offset);
	m_picarats = fileUtils::read2Byte(buffer, offset);
	m_currentHintCoins = fileUtils::read2Byte(buffer, offset);
	m_chapter = fileUtils::read1Byte(buffer, offset);
	uint8_t nameLength = fileUtils::read1Byte(buffer, offset);

	for (size_t i = 0; i < nameLength; i++) {
		m_name.append(1, fileUtils::read4Byte(buffer, offset));
	}

	m_puzzles.reserve(m_puzzleCount);

	for (size_t i = 0; i < m_puzzleCount; i++) {
		m_puzzles.emplace_back(buffer.at(offset));
	}

	m_hintCoins.reserve(m_hintCoinCount);

	for (size_t i = 0; i < m_hintCoinCount; i++) {
		m_hintCoins.emplace_back(buffer.at(offset));
	}
}

void Save::writeCommonData(fileUtils::buffer& buffer) {
	fileUtils::write4Byte(buffer, m_magicNumber);
	fileUtils::write1Byte(buffer, m_id);
	fileUtils::write4Byte(buffer, m_playTime);
	fileUtils::write2Byte(buffer, m_picarats);
	fileUtils::write2Byte(buffer, m_currentHintCoins);
	fileUtils::write1Byte(buffer, m_chapter);
	fileUtils::write1Byte(buffer, (uint8_t)m_name.length());

	for (uint8_t i = 0; i < m_name.length(); i++) {
		fileUtils::write4Byte(buffer, m_name.at(i));
	}

	for (size_t i = 0; i < m_puzzles.size(); i++) {
		fileUtils::write1Byte(buffer, m_puzzles.at(i));
	}

	for (size_t i = 0; i < m_hintCoins.size(); i++) {
		fileUtils::write1Byte(buffer, m_hintCoins.at(i));
	}
}
