#pragma once

#include <Game/Save.h>

class Layton1Save : public Save {
public:
	Layton1Save(const fileUtils::path& inputFile);
	void save();
	void static create(const std::string name);
};
