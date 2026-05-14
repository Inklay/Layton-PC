#include "Game/Layton1/Layton1Save.h"

Layton1Save::Layton1Save(fileUtils::path inputFile) {
	const fileUtils::buffer buffer = fileUtils::readBin(inputFile);
	size_t offset = 0;

	m_filePath = inputFile;
	m_id = LAYTON1;
	m_puzzleCount = 162;
	m_hintCoinCount = 200;

	readCommonData(buffer, offset);
}

Layton1Save::Layton1Save(const std::u32string name, fileUtils::path file) {
	m_name = name;
	m_id = LAYTON1;
	m_picarats = 0;
	m_playTime = 0;
	m_chapter = 0;
	m_currentHintCoins = 10;
	m_filePath = file;

	m_puzzles.reserve(m_puzzleCount);

	for (size_t i = 0; i < m_puzzleCount; i++) {
		m_puzzles.emplace_back(0);
	}

	m_hintCoins.reserve(m_hintCoinCount);

	for (size_t i = 0; i < m_hintCoinCount; i++) {
		m_hintCoins.emplace_back(0);
	}

	save();
}

void Layton1Save::save() {
	fileUtils::buffer buffer;
	buffer.reserve(m_puzzleCount + m_hintCoinCount + 100);

	writeCommonData(buffer);

	fileUtils::writeBin(buffer, m_filePath);
}
