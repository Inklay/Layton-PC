#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Scene.h"
#include "Game/Game.h"

AnimatedSprite::AnimatedSprite(const fileUtils::path& file, Scene* scene, SDL_FRect transform, SDL_FRect subTexture) :
	Sprite(scene, transform, subTexture),
	m_anim(Anim(scene->m_game->m_gameFolder / file)),
	m_frameIndex(0),
	m_frames(0)
{
	for (auto& imageIdx : m_anim.m_imageIdx) {
		if (m_surfaces.count(imageIdx) == 0) {
			m_surfaces.insert({ imageIdx, IMG_Load(((scene->m_game->m_gameFolder / file.parent_path() / file.stem().stem()).string() + "." + std::to_string(imageIdx) + ".png").c_str()) });
			m_textures.insert({ imageIdx, SDL_CreateTextureFromSurface(scene->m_game->m_renderer, m_surfaces.at(imageIdx)) });
			SDL_SetTextureScaleMode(m_textures.at(imageIdx), SDL_SCALEMODE_NEAREST);
		}
	}
}

void AnimatedSprite::draw() {
	if (m_frames == m_anim.m_framesUnk.at(m_frameIndex)) {
		m_frames = 0;
		m_frameIndex++;

		if (m_frameIndex == m_anim.m_framesUnk.size()) {
			m_frameIndex = 0;
		}
	}


	const SDL_FRect* subTexture = m_subTexture.h < 0 || m_subTexture.w < 0 ? nullptr : reinterpret_cast<const SDL_FRect*>(&m_subTexture);
	SDL_RenderTexture(m_scene->m_game->m_renderer, m_textures.at(m_anim.m_imageIdx.at(m_frameIndex)), subTexture, reinterpret_cast<const SDL_FRect*>(&m_transform));

	m_frames++;
}

void AnimatedSprite::unload() {
	for (auto& texture : m_textures) {
		SDL_DestroyTexture(texture.second);
	}

	for (auto& surface : m_surfaces) {
		SDL_DestroySurface(surface.second);
	}

	m_surfaces.clear();
	m_textures.clear();
}
