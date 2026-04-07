#include "Game/Scene.h"

Scene::Scene(Type type) :
	m_type(type)
{
}

void Scene::unload() {
	for (auto& it : m_sprites) {
		it.second->unload();
		it.second.release();
	}

	m_sprites.clear();
}

Scene::Type Scene::type() const {
	return m_type;
}
