#include "Game/Layton1/Layton1Save.h"

Layton1Save::Layton1Save(const fileUtils::path& inputFile) {
	const fileUtils::buffer buffer = fileUtils::readBin(inputFile);
	size_t offset = 0;

	m_id = UNKNOWN;
	m_puzzleCount = 162;

	readCommonData(buffer, offset, LAYTON1);

	if (m_id == UNKNOWN) {
		return;
	}
}


void Layton1Save::save() {

}

void Layton1Save::create(const std::string name) {

}
