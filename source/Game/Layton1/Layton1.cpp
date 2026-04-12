#include "Game/Layton1/Layton1.h"
#include "Game/Layton1/Layton1Save.h"
#include "Game/Layton1/Scene/TitleScreen.h"
#include "Game/Layton1/Scene/CreateSave.h"

Layton1::Layton1(const fileUtils::path& assetsPath, SDL_Window& window) :
	Game(assetsPath, "LAYTON1", window)
{
	getSaves();
	init();
	m_scenes.at(Scene::TITLE_SCREEN).get()->load();
;}

void Layton1::init() {
	m_scenes[Scene::TITLE_SCREEN] = std::make_unique<Layton1Scene::TitleScreen>(this);
	m_scenes[Scene::CREATE_SAVE] = std::make_unique<Layton1Scene::CreateSave>(this);

	m_sfx = {
		{ "level5Logo", "0"},
		{ "titleScreenClick", "1"},
		{ "keyboardSwitch", "2"},
		{ "success", "4"},
		{ "info", "5"},
		{ "step", "8"},
		{ "stepLoud", "9"},
		{ "doorOpen", "10"},
		{ "doorClose", "11"},
		{ "puzzleStart", "12"},
		{ "puzzleEnd", "13"},
		{ "puzzleSuccess", "14"},
		{ "puzzleFailure", "15"},
		{ "keyboardBackspace", "18"},
		{ "keyboardOk", "19"},
		{ "keyboardChar", "99"},
	};
}

void Layton1::getSaves() {
	m_saves.reserve(3);

	for (int i = 0; i < 3; i++) {
		fileUtils::path path = std::filesystem::current_path() / "saves/LAYTON1" / (std::to_string(i) + ".luke");
		if (std::filesystem::exists(path)) {
			m_saves.emplace_back(std::make_unique<Layton1Save>(path));
		} else {
			m_saves.emplace_back(nullptr);
		}
	}
}
