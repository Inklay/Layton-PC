#include "Game/Layton1/Layton1.h"
#include "Game/Layton1/Layton1Save.h"
#include "Game/Layton1/Scene/Scenes.h"
#include "Game/Layton1/Scene/Puzzle/Puzzles.h"
#include "Game/Scene/Cinematic.h"

Layton1::Layton1(const fileUtils::path& assetsPath, SDL_Window& window) :
	Game(assetsPath, "LAYTON1", window)
{
	getSaves();
	init();
	m_scenes.at("titleScreen").get()->load();
}

void Layton1::init() {
	m_scenes["titleScreen"] = std::make_unique<Layton1Scene::TitleScreen>(this);
	m_scenes["createSave"] = std::make_unique<Layton1Scene::CreateSave>(this);
	m_scenes["intro_letter"] = std::make_unique<Layton1Scene::Intro>(this);
	m_scenes["intro_cinematic"] = std::make_unique<Cinematic>(this, "video/m1.mp4", std::vector<fileUtils::path>{ "stream/M1.wav", "sound/SEQ_BG_017.wav" }, "intro_carDialogue1", 0.5f, 0.5f);
	m_scenes["intro_carDialogue1"] = std::make_unique<Layton1Scene::IntroCarDialogue1>(this);
	m_scenes["intro_carDialogue2"] = std::make_unique<Layton1Scene::IntroCarDialogue2>(this);

	m_scenes["puzzle1"] = std::make_unique<Layton1Scene::Puzzle1>(this, "intro_carDialogue2");

	m_sfx = {
		{ "level5Logo", "0"},
		{ "titleScreenClick", "1"},
		{ "keyboardSwitch", "2"},
		{ "hideHints", "3"},
		{ "success", "4"},
		{ "info", "5"},
		{ "puzzleClicked", "7"},
		{ "step", "8"},
		{ "stepLoud", "9"},
		{ "doorOpen", "10"},
		{ "doorClose", "11"},
		{ "puzzleStart", "12"},
		{ "puzzleEnd", "13"},
		{ "puzzleSuccess", "14"},
		{ "puzzleFailure", "15"},
		{ "showHints", "16"},
		{ "clear", "18"},
		{ "keyboardOk", "19"},
		{ "introSmallBang", "39"},
		{ "introBang", "43"},
		{ "unlockHint", "83"},
		{ "keyboardChar", "99"},
		{ "puzzlePicaratDecrease", "130"},
		{ "switchHint", "131"},
		{ "lukeAnswer1", "153"},
		{ "lukeAnswer2", "154"},
		{ "lukeAnswer3", "155"},
		{ "lukeWrong1", "156"},
		{ "lukeWrong2", "157"},
		{ "lukeWrong3", "158"},
		{ "lukeRight1", "159"},
		{ "lukeRight2", "160"},
		{ "lukeRight3", "161"},
		{ "laytonAnswer1", "162"},
		{ "laytonAnswer2", "163"},
		{ "laytonAnswer3", "164"},
		{ "laytonWrong1", "165"},
		{ "laytonWrong2", "166"},
		{ "laytonWrong3", "167"},
		{ "laytonRight1", "168"},
		{ "laytonRight1", "169"},
		{ "laytonRight2", "170"}
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
