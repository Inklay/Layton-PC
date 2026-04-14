#include "Game/Layton1/Scene/Intro.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Intro::Intro(Game* game) :
		Scene(game)
	{
	}

	void Intro::load() {
		Scene::load();

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/ara_top.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_20.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "firstText", std::make_unique<Sprite>("font/fontevent.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });

		m_texts.insert({ "firstText", getText("itext/fr/t_0.txt", U"%s", m_game->m_save->m_name)});

		m_bottomScreenRect = SDL_FRect{ 0, HALF_HEIGHT * m_game->m_windowMultiplier, WIDTH * m_game->m_windowMultiplier, HALF_HEIGHT * m_game->m_windowMultiplier };
		m_fading = true;
		m_fadeInSeconds = 0.3f;
	}

	void Intro::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("bottomBackground")->draw();
		if (m_lastTick != 0) {
			m_timer += SDL_GetTicks() - m_lastTick;
		}

		switch (m_step) {
			case 0:
				m_bottomOpacity = (uint8_t)(m_timer * 150 / 1500);
				if (m_bottomOpacity >= 150) {
					m_bottomOpacity = 150;
					m_step = 1;
				}
				break;
			case 1:
				m_sprites.at("firstText")->drawText(m_texts.at("firstText"));
		} 
		
		SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
		SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, m_bottomOpacity);
		SDL_RenderFillRect(m_game->m_renderer, &m_bottomScreenRect);

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void Intro::handleClick(const std::string& spriteName, SDL_Event event) {
	}
};
