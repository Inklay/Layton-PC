#include "Game/Layton1/Scene/IntroCarDialogue1.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	IntroCarDialogue1::IntroCarDialogue1(Game* game) :
		Scene(game)
	{
	}

	void IntroCarDialogue1::load() {
		m_sprites.insert({ "topBackground", std::make_unique<FadingSprite>("bg/I_car4.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }, FadingSprite::Mode::IN, 500, -1000) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "anim1", std::make_unique<AnimatedSprite>("ani/intro2_wani1_new.gfx.anim", this, SDL_FRect{ WIDTH - 50, HALF_HEIGHT, 50, 70 }) });
		m_sprites.insert({ "anim2", std::make_unique<AnimatedSprite>("ani/intro2_wani2_new.gfx.anim", this, SDL_FRect{ 101, HALF_HEIGHT + 19, 46, 38 }) });
		m_sprites.insert({ "bottomFading", std::make_unique<FadingSprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, FadingSprite::Mode::OUT, 500, -2000) });
		
		m_dialogue.load({
			{ "luke", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_face_new.bg2notalk.anim", "ani/luke_face_new.bg2talk.anim", SDL_FRect{ 167, HALF_HEIGHT + 38, 40, 32 } },
			{ "layton", "ani/fr/layton_n.png", "ani/event_window_1.2.png", "ani/layton_face_new.bg2notalk.anim", "ani/layton_face_new.bg2talk.anim", SDL_FRect{ 43, HALF_HEIGHT + 25, 40, 48 } }
		});

		Scene::load();
	}

	void IntroCarDialogue1::render() {
		m_sprites.at("topBackground")->draw();
		m_sprites.at("bottomBackground")->draw();
		m_sprites.at("anim1")->draw();
		m_sprites.at("anim2")->draw();
		m_dialogue.draw();

		//m_sprites.at("bottomFading")->draw();

		if (true) {
			m_dialogue.setVisible(true);
			switch (m_dialogueProgression) {
				case 0:
					m_dialogue.setDialogue("itext/fr/t_1.txt", "luke", "stream/SP_V_OP_101.wav");
					m_dialogueProgression = 1;
					break;
			}
		}

		Scene::render();
	}

	void IntroCarDialogue1::handleClick(const std::string& spriteName, SDL_Event event) {
	}
};
