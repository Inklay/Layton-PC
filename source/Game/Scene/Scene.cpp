#include "Game/Scene.h"

Scene::Scene(Type type) :
	m_type(type)
{
}

void Scene::unload() {
	for (std::pair<std::string, Sprite> it : m_sprites) {
		SDL_DestroyTexture(it.second.m_texture);
		SDL_DestroySurface(it.second.m_surface);
	}

	m_sprites.clear();
}

Scene::Type Scene::type() const {
	return m_type;
}
