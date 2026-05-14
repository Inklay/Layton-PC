#include "Game/Layton1/Scene/TitleDrop.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	TitleDrop::TitleDrop(Game* game) :
		Scene(game, 0.5f, 0.5f)
	{
	}

	void TitleDrop::load() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/title_top.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/title_bottom.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "gameLogo", std::make_unique<Sprite>("ani/fr/title_logo.png", this, SDL_FRect{ centerXPos(240), centerBottomPos(104), 240, 104}) });
		m_sprites.insert({ "touch", std::make_unique<Sprite>("ani/fr/title.png", this, SDL_FRect{ centerXPos(64), centerBottomPos(16), 64, 16}, true) });

		playBGM("stream/BGM_09.wav");
		m_sprites.at("gameLogo")->fade({ 1000, -1000, Sprite::FadingMode::IN });

		m_fading = true;
		Scene::load();
	}

	void TitleDrop::render() {
		m_sprites.at("topBackground")->draw();;
		m_sprites.at("bottomBackground")->draw();
		m_sprites.at("gameLogo")->draw();

		if (!m_sprites.at("gameLogo")->m_fading && m_logoState == FADING) {
			m_logoState = TRANSLATING;
			m_sprites.at("gameLogo")->translate(Sprite::TranslationInfo{ 1500, 0.0f, -HALF_HEIGHT });
		}

		if (!m_sprites.at("gameLogo")->m_translating && m_logoState == TRANSLATING) {
			m_logoState = DONE;
		}

		if (m_logoState == DONE) {
			m_touchCounter++;

			if (m_touchCounter > 0) {
				m_sprites.at("touch")->draw();

				if (m_touchCounter == 16) {
					m_touchCounter = -15;
				}
			}
		}

		Scene::render();
	}

	void TitleDrop::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_logoState == DONE && !m_fading) {
			playSFX("titleScreenClick");
			fadeToNextScene("arrivingAtStMystere_cinematic");
		}
	}
};
