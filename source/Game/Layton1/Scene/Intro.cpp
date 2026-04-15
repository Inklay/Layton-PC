#include "Game/Layton1/Scene/Intro.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Sprite/FadingSprite.h"
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
		m_sprites.insert({ "firstText", std::make_unique<TextSprite>("font/fontevent.png", "itext/fr/t_0.txt", this, SDL_FRect{ -1, -1, WIDTH, HALF_HEIGHT }, SDL_Color{255, 255, 255}, U"%s", m_game->m_save->m_name) });
		m_sprites.insert({ "fadingText1", std::make_unique<FadingSprite>("ani/fr/op_message.0.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 30, 226, 12}, 1000, 0) });
		m_sprites.insert({ "fadingText2", std::make_unique<FadingSprite>("ani/fr/op_message.1.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 55, 226, 12}, 1000, -2000) });
		m_sprites.insert({ "fadingText3", std::make_unique<FadingSprite>("ani/fr/op_message.2.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 80, 226, 12}, 1000, -4000) });
		m_sprites.insert({ "fadingText4", std::make_unique<FadingSprite>("ani/fr/op_message.3.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 105, 226, 12}, 1000, -6000) });
		m_sprites.insert({ "fadingText5", std::make_unique<FadingSprite>("ani/fr/op_message.4.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 145, 226, 12}, 1000, -9000) });

		m_sprites.at("firstText")->m_opacity = 0;
		m_bottomScreenRect = SDL_FRect{ 0, HALF_HEIGHT * m_game->m_windowMultiplier, WIDTH * m_game->m_windowMultiplier, HALF_HEIGHT * m_game->m_windowMultiplier };
		m_fading = true;
		m_fadeInSeconds = 0.3f;
		m_fadeOutSeconds = 0.3f;
	}

	void Intro::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("bottomBackground")->draw();
		if (m_lastTick != 0) {
			m_timer += SDL_GetTicks() - m_lastTick;
		}

		SDL_SetRenderDrawBlendMode(m_game->m_renderer, SDL_BLENDMODE_MUL);
		SDL_SetRenderDrawColor(m_game->m_renderer, 0, 0, 0, m_bottomOpacity);
		SDL_RenderFillRect(m_game->m_renderer, &m_bottomScreenRect);

		switch (m_step) {
			case 0:
				m_bottomOpacity = (uint8_t)(m_timer * 180 / 1500);
				if (m_bottomOpacity >= 180) {
					m_bottomOpacity = 180;
					m_step = 1;
					m_timer = 0;
				}
				break;
			case 1: {
				Sprite* text = m_sprites.at("firstText").get();
				text->draw();
				text->m_opacity = (int)(m_timer * 255 / 1000);
				if (text->m_opacity >= 255) {
					text->m_opacity = 255;
					m_step = 2;
					m_timer = 0;
				}
				break;
			}
			case 2:
				m_sprites.at("firstText")->draw();
				if (m_timer >= 1500) {
					m_step = 3;
					m_timer = 0;
				}
				break;
			case 3: {
				Sprite* text = m_sprites.at("firstText").get();
				text->draw();
				text->m_opacity = 255 - (int)(m_timer * 255 / 1000);
				if (text->m_opacity <= 0) {
					text->m_opacity = 0;
					m_step = 4;
					m_timer = 0;
					playBGM("stream/SP_V_EP_001.wav");
				}
				break;
			}
			case 4:
				m_sprites.at("fadingText1")->draw();
				m_sprites.at("fadingText2")->draw();
				m_sprites.at("fadingText3")->draw();
				m_sprites.at("fadingText4")->draw();
				m_sprites.at("fadingText5")->draw();
				if (m_timer >= 10500) {
					pauseBGM();
					fadeToNextScene(CINEMATIC);
				}
				break;
		} 

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void Intro::handleClick(const std::string& spriteName, SDL_Event event) {
	}
};
