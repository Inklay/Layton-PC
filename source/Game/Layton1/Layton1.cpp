#include "Game/Layton1/Layton1.h"
#include "Game/Layton1/Layton1Save.h"
#include "Game/Layton1/Scene/TitleScreen.h"

Layton1::Layton1(const fileUtils::path& assetsPath, SDL_Window& window) :
	Game(assetsPath, "LAYTON1", window)
{
	getSaves();
	createScenes();
	m_scenes.at(Scene::TITLE_SCREEN).get()->load();
;}

void Layton1::createScenes() {
	m_scenes[Scene::TITLE_SCREEN] = std::make_unique<Layton1Scene::TitleScreen>(this);
}

void Layton1::getSaves() {
	m_saves.reserve(3);

	for (int i = 0; i < 3; i++) {
		fileUtils::path path = std::filesystem::current_path() / "saves/LAYTON1" / (std::to_string(i) + ".luke");
		if (std::filesystem::exists(path)) {
			m_saves.emplace_back(std::make_unique<Layton1Save>(path));
		}
		else {
			m_saves.emplace_back(nullptr);
		}
	}
}
