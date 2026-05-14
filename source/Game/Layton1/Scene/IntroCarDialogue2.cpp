#include "Game/Layton1/Scene/IntroCarDialogue2.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	IntroCarDialogue2::IntroCarDialogue2(Game* game) :
		Scene(game, 0.5f, 0.8f)
	{
	}

	void IntroCarDialogue2::load() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/I_car4.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "topBackground2", std::make_unique<Sprite>("bg/I_car5.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "anim1", std::make_unique<AnimatedSprite>("ani/intro2_wani1_new.gfx.anim", this, SDL_FRect{ WIDTH - 50, HALF_HEIGHT, 50, 70 }) });
		m_sprites.insert({ "anim2", std::make_unique<AnimatedSprite>("ani/intro2_wani2_new.gfx.anim", this, SDL_FRect{ 101, HALF_HEIGHT + 19, 46, 38 }) });
		m_sprites.insert({ "anim3", std::make_unique<AnimatedSprite>("ani/intro1_wani1_new.gfx.anim", this, SDL_FRect{ 40, HALF_HEIGHT + 20, 166, 50}) });
		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground2", std::make_unique<Sprite>("bg/ibg_1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		
		m_dialogue.load({
			{ "luke", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_face_new.bg2notalk.anim", "ani/luke_face_new.bg2talk.anim", SDL_FRect{ 167, HALF_HEIGHT + 38, 40, 32 } },
			{ "luke2", "ani/fr/luke.png", "ani/event_window_1.1.png", "ani/luke_face_new.bg1notalk.anim", "ani/luke_face_new.bg1notalk.anim", SDL_FRect{ 175, HALF_HEIGHT + 71, 32, 20 } },
			{ "layton", "ani/fr/layton_n.png", "ani/event_window_1.2.png", "ani/layton_face_new.bg2notalk.anim", "ani/layton_face_new.bg2talk.anim", SDL_FRect{ 43, HALF_HEIGHT + 25, 40, 48 } },
			{ "layton2", "ani/fr/layton_n.png", "ani/event_window_1.2.png", "ani/layton_face_new.bg1notalk.anim", "ani/layton_face_new.bg1talk.anim", SDL_FRect{ 58, HALF_HEIGHT + 43, 32, 32 } },
		});

		m_sprites.at("topBackground")->fade({ 500, -1000, Sprite::FadingMode::IN });
		m_sprites.at("bottomFading")->fade({ 500, -2000, Sprite::FadingMode::OUT });

		Scene::load();
	}

	void IntroCarDialogue2::render() {
		m_sprites.at("topBackground")->draw();

		if (m_bottomBackground == 0) {
			m_sprites.at("bottomBackground")->draw();
			m_sprites.at("anim1")->draw();
			m_sprites.at("anim2")->draw();
		} else {
			m_sprites.at("bottomBackground2")->draw();
			m_sprites.at("anim3")->draw();
		}

		if (m_timeElapsed >= 2500 && m_dialogueProgression == 0) {
			m_dialogue.setVisible(true);
			m_dialogue.setCharacterVisible("luke", true);
			m_dialogue.setCharacterVisible("layton", true);
			m_dialogue.setDialogue("itext/fr/t_21.txt", "luke", { "stream/SP_V_OP_137.wav" });
			m_dialogueProgression++;
		}

		m_dialogue.draw();
		m_sprites.at("bottomFading")->draw();

		Scene::render();
	}

	void IntroCarDialogue2::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_dialogueProgression >= 1) {
			if (m_dialogue.waiting()) {
				if (m_dialogue.next()) {
					return;
				}

				switch (m_dialogueProgression) {
					case 1:
						m_dialogue.setDialogue("itext/fr/t_22.txt", "layton", { "stream/SP_V_OP_138.wav" });
						m_dialogueProgression++;
						break;
					case 2:
						m_dialogue.setDialogue("itext/fr/t_23.txt", "luke2", { "stream/SP_V_OP_139.wav" });
						m_bottomBackground = 1;
						m_dialogue.setCharacterVisible("luke", false);
						m_dialogue.setCharacterVisible("layton", false);
						m_dialogue.setCharacterVisible("luke2", true);
						m_dialogue.setCharacterVisible("layton2", true);
						m_dialogueProgression++;
						break;
					case 3:
						m_dialogue.setDialogue("itext/fr/t_24.txt", "layton2", { "stream/SP_V_OP_140.wav" });
						m_dialogueProgression++;
						break;
					case 4:
						m_dialogue.setDialogue("itext/fr/t_25.txt", "layton", { "stream/SP_V_OP_141.wav" });
						m_bottomBackground = 0;
						m_dialogue.setCharacterVisible("luke2", false);
						m_dialogue.setCharacterVisible("layton2", false);
						m_dialogue.setCharacterVisible("luke", true);
						m_dialogue.setCharacterVisible("layton", true);
						m_dialogueProgression++;
						break;
					case 5:
						fadeToNextScene("titleDrop");
				}
			} else {
				m_dialogue.skip();
			}
		}
	}
};
