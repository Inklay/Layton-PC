#include "Game/Layton1/Scene/Intro.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Intro::Intro(Game* game) :
		Scene(game, 0.5f, 0.5f),
		m_bottomScreenRect(SDL_FRect{ 0, HALF_HEIGHT * game->m_windowMultiplier, WIDTH * game->m_windowMultiplier, HALF_HEIGHT * game->m_windowMultiplier })
	{
	}

	void Intro::load() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/ara_top.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_20.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "firstText", std::make_unique<TextSprite>("font/fontevent.png", "itext/fr/t_0.txt", this, SDL_FRect{ -1, -1, WIDTH, HALF_HEIGHT }, SDL_Color{255, 255, 255, 255}, U"%s", m_game->m_save->m_name) });
		m_sprites.insert({ "fadingText1", std::make_unique<Sprite>("ani/fr/op_message.0.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 30, 226, 12}) });
		m_sprites.insert({ "fadingText2", std::make_unique<Sprite>("ani/fr/op_message.1.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 55, 226, 12}) });
		m_sprites.insert({ "fadingText3", std::make_unique<Sprite>("ani/fr/op_message.2.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 80, 226, 12}) });
		m_sprites.insert({ "fadingText4", std::make_unique<Sprite>("ani/fr/op_message.3.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 105, 226, 12}) });
		m_sprites.insert({ "fadingText5", std::make_unique<Sprite>("ani/fr/op_message.4.png", this, SDL_FRect{ centerXPos(226), HALF_HEIGHT + 145, 226, 12}) });

		m_sprites.at("fadingText1")->fade({ 1000, 0, Sprite::FadingMode::IN });
		m_sprites.at("fadingText2")->fade({ 1000, -2000, Sprite::FadingMode::IN });
		m_sprites.at("fadingText3")->fade({ 1000, -4000, Sprite::FadingMode::IN });
		m_sprites.at("fadingText4")->fade({ 1000, -6000, Sprite::FadingMode::IN });
		m_sprites.at("fadingText5")->fade({ 1000, -9000, Sprite::FadingMode::IN });
		m_fading = true;

		Scene::load();
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
					m_step++;
					m_timer = 0;
				}
				break;
			case 1:
				m_sprites.at("firstText")->fade({ 1000, 0, Sprite::FadingMode::IN });
				m_step++;
				break;
			case 2:
				if (!m_sprites.at("firstText")->m_fading) {
					m_sprites.at("firstText")->fade({ 1000, -1500, Sprite::FadingMode::OUT });
					m_step++;
				}
				m_sprites.at("firstText")->draw();
				break;
			case 3:
				if (!m_sprites.at("firstText")->m_fading) {
					m_step++;
					m_timer = 0;
					playBGM("stream/SP_V_EP_001.wav");
				}
				m_sprites.at("firstText")->draw();
				break;
			case 4:
				m_sprites.at("fadingText1")->draw();
				m_sprites.at("fadingText2")->draw();
				m_sprites.at("fadingText3")->draw();
				m_sprites.at("fadingText4")->draw();
				m_sprites.at("fadingText5")->draw();
				if (m_timer >= 10450) {
					pauseBGM();
					fadeToNextScene("intro_cinematic");
				}
				break;
		} 

		Scene::render();
	}
};
