#include "Game/Layton1/Layton1.h"
#include "Game/Layton1/Layton1Save.h"
#include "Game/Layton1/Scene/TitleScreen.h"
#include "Game/Layton1/Scene/CreateSave.h"
#include "Game/Layton1/Scene/Intro.h"

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
	m_scenes[Scene::INTRO] = std::make_unique<Layton1Scene::Intro>(this);

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
	m_saves.resize(3);

	for (int i = 0; i < 3; i++) {
		fileUtils::path path = std::filesystem::current_path() / "saves/LAYTON1" / (std::to_string(i) + ".luke");
		if (std::filesystem::exists(path)) {
			m_saves.emplace_back(std::make_unique<Layton1Save>(path));
		} else {
			m_saves.emplace_back(nullptr);
		}
	}
}

void Layton1::addSave(const std::u32string name) {
	int idx = 0;

	for (int idx = 0; idx < 3; idx++) {
		if (m_saves.at(idx).get() == nullptr) {
			break;
		}
	}

	fileUtils::path path = std::filesystem::current_path() / "saves" / m_name / (std::to_string(idx + 1) + ".luke");
	m_saves.at(idx) = std::make_unique<Layton1Save>(name, path);
	m_save = m_saves.at(idx).get();
}
