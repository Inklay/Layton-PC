#include "Game/Layton1/Layton1Save.h"

Layton1Save::Layton1Save(fileUtils::path inputFile) {
	const fileUtils::buffer buffer = fileUtils::readBin(inputFile);
	size_t offset = 0;

	m_filePath = inputFile;
	m_id = UNKNOWN;
	m_puzzleCount = 162;

	readCommonData(buffer, offset, LAYTON1);

	if (m_id == UNKNOWN) {
		return;
	}
}

Layton1Save::Layton1Save(const std::string name, fileUtils::path file) {
	m_name = name;
	m_id = LAYTON1;
	m_picarats = 0;
	m_playTime = 0;
	m_filePath = file;

	m_puzzles.reserve(m_puzzleCount);

	for (size_t i = 0; i < m_puzzleCount; i++) {
		m_puzzles.emplace_back(0);
	}

	save();
}

void Layton1Save::save() {
	fileUtils::buffer buffer;
	buffer.reserve(m_puzzleCount + 100);

	fileUtils::write4Byte(buffer, m_magicNumber);
	fileUtils::write1Byte(buffer, LAYTON1);
	fileUtils::write4Byte(buffer, m_playTime);
	fileUtils::write2Byte(buffer, m_picarats);
	fileUtils::write1Byte(buffer, (uint8_t)m_name.length());

	for (uint8_t i = 0; i < m_name.length(); i++) {
		fileUtils::write1Byte(buffer, m_name.at(i));
	}

	for (size_t i = 0; i < m_puzzles.size(); i++) {
		fileUtils::write1Byte(buffer, m_puzzles.at(i));
	}

	fileUtils::writeBin(buffer, m_filePath);
}
