#include "Game/Layton1.h"
#include "Game/Scene/Layton1/TitleScreen.h"

Layton1::Layton1(const fileUtils::path& assetsPath, SDL_Window& window) :
	Game(assetsPath, "Curious Village", window)
{
	createScenes();
	m_scenes.at(Scene::TITLE_SCREEN).get()->load(m_gameFolder, m_renderer, m_windowMultiplier);
}

void Layton1::createScenes() {
	m_scenes[Scene::TITLE_SCREEN] = std::make_unique<Layton1Scene::TitleScreen>();
}
