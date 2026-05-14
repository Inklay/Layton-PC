#include "Game/layton1/Scene/map/DrawBridge.h"
#include "Game/Game.h"
#include "Game/Sprite/AnimatedSprite.h"

namespace Layton1Scene {
	DrawBridge::DrawBridge(Game* game) :
		Map(game)
	{}

	void DrawBridge::load() {
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/room_1_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "anim1", std::make_unique<AnimatedSprite>("ani/r1_bgobj_1.gfx.anim", this, SDL_FRect{ 158, HALF_HEIGHT, 6, 30 }, true) });
		m_sprites.insert({ "anim2", std::make_unique<AnimatedSprite>("ani/r1_bgobj_2.gfx.anim", this, SDL_FRect{ 65, HEIGHT - 28, 32, 32 }, true) });
		m_sprites.insert({ "anim3", std::make_unique<AnimatedSprite>("ani/r1_bgobj_3.gfx.anim", this, SDL_FRect{ 129, HEIGHT - 28, 16, 32 }, true) });

		if (m_game->m_save->m_chapter == 0) {
			m_sprites.insert({ "car", std::make_unique<Sprite>("ani/obj_2.png", this, SDL_FRect{ 49, HALF_HEIGHT + 109, 48, 40 }, true) });
			m_sprites.insert({ "laytonAndLuke", std::make_unique<AnimatedSprite>("ani/obj_1.gfx.anim", this, SDL_FRect{ 20, HALF_HEIGHT + 111, 30, 48 }, true) });
			m_sprites.insert({ "franco", std::make_unique<AnimatedSprite>("ani/obj_3.gfx.anim", this, SDL_FRect{ 158, HALF_HEIGHT + 109, 26, 42 }, true) });
		}

		Map::load();
	}

	void DrawBridge::render() {
		m_sprites.at("bottomBackground")->draw();
		m_sprites.at("anim1")->draw();
		m_sprites.at("anim2")->draw();
		m_sprites.at("anim3")->draw();

		if (m_game->m_save->m_chapter == 0) {
			m_sprites.at("car")->draw();
			m_sprites.at("laytonAndLuke")->draw();
			m_sprites.at("franco")->draw();
		}

		Map::render();
	}

	void DrawBridge::handleClick(const std::string& spriteName, SDL_Event event) {

	}
};
