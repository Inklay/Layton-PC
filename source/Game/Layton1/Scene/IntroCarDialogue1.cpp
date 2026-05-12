#include "Game/Layton1/Scene/IntroCarDialogue1.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	IntroCarDialogue1::IntroCarDialogue1(Game* game) :
		Scene(game)
	{
	}

	void IntroCarDialogue1::load() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/I_car4.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "topBackground2", std::make_unique<Sprite>("bg/I_car5.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ibg_2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "anim1", std::make_unique<AnimatedSprite>("ani/intro2_wani1_new.gfx.anim", this, SDL_FRect{ WIDTH - 50, HALF_HEIGHT, 50, 70 }) });
		m_sprites.insert({ "anim2", std::make_unique<AnimatedSprite>("ani/intro2_wani2_new.gfx.anim", this, SDL_FRect{ 101, HALF_HEIGHT + 19, 46, 38 }) });
		m_sprites.insert({ "anim3", std::make_unique<AnimatedSprite>("ani/intro4_wani1_new.gfx.anim", this, SDL_FRect{ 0, HALF_HEIGHT, 130, 100}) });
		m_sprites.insert({ "anim4", std::make_unique<AnimatedSprite>("ani/intro4_wani2_new.gfx.anim", this, SDL_FRect{ 132, HALF_HEIGHT + 6, 44, 104 }) });
		m_sprites.insert({ "anim5", std::make_unique<AnimatedSprite>("ani/intro3_wani3_new.gfx.anim", this, SDL_FRect{ 102, HALF_HEIGHT + 42, 62, 50 }) });
		m_sprites.insert({ "anim6", std::make_unique<AnimatedSprite>("ani/intro3_wani1_new.gfx.anim", this, SDL_FRect{ 0, HALF_HEIGHT + 14, 46, 80 }) });
		m_sprites.insert({ "anim7", std::make_unique<AnimatedSprite>("ani/intro3_wani2_new.gfx.anim", this, SDL_FRect{ 210, HALF_HEIGHT + 15, 46, 80}) });
		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground2", std::make_unique<Sprite>("bg/ibg_17.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackground3", std::make_unique<Sprite>("bg/ibg_18.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackground4", std::make_unique<Sprite>("bg/ibg_4.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackground5", std::make_unique<Sprite>("bg/ibg_3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		
		m_dialogue.load({
			{ "luke", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_face_new.bg2notalk.anim", "ani/luke_face_new.bg2talk.anim", SDL_FRect{ 167, HALF_HEIGHT + 38, 40, 32 } },
			{ "layton", "ani/fr/layton_n.png", "ani/event_window_1.2.png", "ani/layton_face_new.bg2notalk.anim", "ani/layton_face_new.bg2talk.anim", SDL_FRect{ 43, HALF_HEIGHT + 25, 40, 48 } },
			{ "layton_offscreen", "ani/fr/layton_n.png", "ani/event_window_1.0.png", "", "", SDL_FRect{ 0, 0, 0, 0 } },
			{ "layton2", "ani/fr/layton_n.png", "ani/event_window_1.1.png", "ani/layton_face_new.bg4notalk.anim", "ani/layton_face_new.bg4talk.anim", SDL_FRect{ 169, HALF_HEIGHT + 64, 30, 24 } },
			{ "luke2", "ani/fr/luke_n.png", "ani/event_window_1.2.png", "", "", SDL_FRect{ 0, 0, 0 } }
		});

		m_sprites.at("topBackground")->fade({ 500, -1000, Sprite::FadingMode::IN });
		m_sprites.at("bottomFading")->fade({ 500, -2000, Sprite::FadingMode::OUT });

		Scene::load();
#ifdef _DEBUG
		m_dialogueProgression = 5;
		m_dialogue.setVisible(true);
#endif
	}

	void IntroCarDialogue1::render() {
		if (m_dialogueProgression < 6) {
			m_sprites.at("topBackground")->draw();
		} 
		if (m_dialogueProgression >= 5) {
			m_sprites.at("topBackground2")->draw();

			if (!m_sprites.at("topBackground2")->m_fading && m_dialogueProgression == 5) {
				m_dialogueProgression++;
				m_dialogue.setDialogue("itext/fr/t_5.txt", "layton2", { "stream/SP_V_OP_109.wav", "stream/SP_V_OP_110.wav" });
				m_dialogue.setCharacterVisible("layton_offscreen", false);
				m_dialogue.setCharacterVisible("layton2", true);
			}
		}
		
		if (m_dialogueProgression < 3 || (m_dialogueProgression >= 10 && m_dialogueProgression < 14)) {
			m_sprites.at("bottomBackground")->draw();
			m_sprites.at("anim1")->draw();
			m_sprites.at("anim2")->draw();
		} else if (m_dialogueProgression == 3) {
			m_sprites.at("bottomBackground2")->draw();
		} else if (m_dialogueProgression == 4 || m_dialogueProgression == 5) {
			m_sprites.at("bottomBackground3")->draw();
		} else if (m_dialogueProgression == 6) {
			m_sprites.at("bottomBackground4")->draw();
			m_sprites.at("anim3")->draw();
			m_sprites.at("anim4")->draw();
		} else if (m_dialogueProgression >= 7 || m_dialogueProgression >= 14) {
			m_sprites.at("bottomBackground5")->draw();
			m_sprites.at("anim5")->draw();
			m_sprites.at("anim6")->draw();
			m_sprites.at("anim7")->draw();
		}

		if (m_timeElapsed >= 2500 && m_dialogueProgression == 0) {
			m_dialogue.setVisible(true);
			m_dialogue.setCharacterVisible("luke", true);
			m_dialogue.setCharacterVisible("layton", true);
			m_dialogue.setDialogue("itext/fr/t_1.txt", "luke", { "stream/SP_V_OP_101.wav" });
			m_dialogueProgression++;
		}

		m_dialogue.draw();
		m_sprites.at("bottomFading")->draw();

		Scene::render();
	}

	void IntroCarDialogue1::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_dialogueProgression >= 1) {
			if (m_dialogue.waiting()) {
				if (m_dialogue.next()) {
					return;
				}

				switch (m_dialogueProgression) {
				case 1:
					m_dialogue.setDialogue("itext/fr/t_2.txt", "layton", { "stream/SP_V_OP_102.wav", "stream/SP_V_OP_103.wav", "stream/SP_V_OP_104.wav" });
					m_dialogueProgression++;
					break;
				case 2:
					m_dialogue.setDialogue("itext/fr/t_3.txt", "layton_offscreen", { "stream/SP_V_OP_105.wav", "stream/SP_V_OP_106.wav" });
					m_dialogue.setCharacterVisible("luke", false);
					m_dialogue.setCharacterVisible("layton", false);
					m_dialogue.setCharacterVisible("layton_offscreen", true);
					playSFX("introBang");
					m_dialogueProgression++;
					break;
				case 3:
					m_dialogue.setDialogue("itext/fr/t_4.txt", "layton_offscreen", { "stream/SP_V_OP_107.wav", "stream/SP_V_OP_108.wav" });
					playSFX("introBang");
					m_dialogueProgression++;
					break;
				case 4:
					m_sprites.at("topBackground")->fade({ 500, 0, Sprite::FadingMode::OUT });
					m_sprites.at("topBackground2")->fade({ 500, -500, Sprite::FadingMode::IN });
					m_dialogueProgression++;
					break;
				case 6:
					m_dialogue.setCharacterVisible("layton2", false);
					m_dialogue.setDialogue("itext/fr/t_6.txt", "layton2", { "stream/SP_V_OP_111.wav" });
					m_dialogueProgression++;
					break;
				case 7:
					m_dialogue.setDialogue("itext/fr/t_7.txt", "luke2", { "stream/SP_V_OP_112.wav" });
					m_dialogueProgression++;
					break;
				case 8:
					m_dialogue.setDialogue("itext/fr/t_8.txt", "layton2", { "stream/SP_V_OP_113.wav", "stream/SP_V_OP_114.wav" });
					m_dialogueProgression++;
					break;
				case 9:
					m_dialogue.setCharacterVisible("layton", true);
					m_dialogue.setCharacterVisible("luke", true);
					m_dialogue.setDialogue("itext/fr/t_9.txt", "luke", { "stream/SP_V_OP_115.wav", "stream/SP_V_OP_116.wav" });
					m_dialogueProgression++;
					break;
				case 10:
					m_dialogue.setDialogue("itext/fr/t_10.txt", "layton", { "stream/SP_V_OP_117.wav", "stream/SP_V_OP_118.wav" });
					m_dialogueProgression++;
					break;
				case 11:
					m_dialogue.setDialogue("itext/fr/t_11.txt", "luke", { "stream/SP_V_OP_119.wav" });
					m_dialogueProgression++;
					break;
				case 12:
					m_dialogue.setDialogue("itext/fr/t_12.txt", "layton", { "stream/SP_V_OP_120.wav" });
					m_dialogueProgression++;
					break;
				case 13:
					m_dialogue.setCharacterVisible("luke", false);
					m_dialogue.setCharacterVisible("layton", false);
					m_dialogue.setDialogue("itext/fr/t_13.txt", "luke2", { "stream/SP_V_OP_121.wav", "stream/SP_V_OP_122.wav" });
					m_dialogueProgression++;
					break;
				case 14:
					m_dialogue.setDialogue("itext/fr/t_14.txt", "layton2", { "stream/SP_V_OP_123.wav", "stream/SP_V_OP_124.wav", "stream/SP_V_OP_125.wav" });
					m_dialogueProgression++;
					break;
				case 15:
					m_dialogue.setDialogue("itext/fr/t_38.txt", "luke2", { "stream/SP_V_OP_126.wav" });
					m_dialogueProgression++;
					break;
				case 16:
					m_dialogue.setDialogue("itext/fr/t_39.txt", "layton2", { "stream/SP_V_OP_127.wav" });
					m_dialogueProgression++;
					break;
				case 17:
					m_dialogue.setDialogue("itext/fr/t_15.txt", "luke2", { "stream/SP_V_OP_128.wav" });
					m_dialogueProgression++;
					break;
				}
			} else {
				m_dialogue.skip();
			}
		}
	}
};
