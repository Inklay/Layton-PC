#include "Game/Layton1/Layton1.h"
#include "Game/Layton1/Layton1Save.h"
#include "Game/Layton1/Scene/TitleScreen.h"

Layton1::Layton1(const fileUtils::path& assetsPath, SDL_Window& window) :
	Game(assetsPath, "LAYTON1", window)
{
	createScenes();
	m_scenes.at(Scene::TITLE_SCREEN).get()->load();
	m_save = std::make_unique<Layton1Save>("test", (std::filesystem::current_path() / "saves/LAYTON1/1.luke"));
;}

void Layton1::createScenes() {
	m_scenes[Scene::TITLE_SCREEN] = std::make_unique<Layton1Scene::TitleScreen>(this);
}
