#include "Game/Layton1/Scene/StMystereEntry.h"
#include "Game/Sprite/Sprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	StMystereEntry::StMystereEntry(Game* game) :
		Scene(game, 0.5f, 0.5f)
	{}

	void StMystereEntry::load() {

		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/ebgs_1.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/ara_1_1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "bottomBackground2", std::make_unique<Sprite>("bg/room_1_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "layton", std::make_unique<Sprite>("ani/layton.5.png", this, SDL_FRect{ 20, HEIGHT - 176, 105, 176}, true) });
		m_sprites.insert({ "layton2", std::make_unique<Sprite>("ani/layton.2.png", this, SDL_FRect{ 20, HEIGHT - 176, 105, 176}, true) });
		m_sprites.insert({ "luke", std::make_unique<Sprite>("ani/luke.1.png", this, SDL_FRect{ 160, HEIGHT - 142, 68, 142 }, true) });
		m_sprites.insert({ "luke2", std::make_unique<Sprite>("ani/luke.3.png", this, SDL_FRect{ 160, HEIGHT - 142, 68, 142 }, true) });

		m_dialogue.load({
			{ "layton", "ani/fr/layton_n.png", "ani/event_window_1.1.png", "", "", SDL_FRect{ 0, 0, 0, 0 } },
			{ "luke", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_e_face.notalk.anim", "ani/luke_e_face.talk.anim", SDL_FRect{ m_sprites.at("luke")->m_transform.x + 20, HEIGHT - 100, 19, 13}},
			{ "luke2", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_e_face.notalk.anim", "ani/luke_e_face.smile.anim", SDL_FRect{ m_sprites.at("luke")->m_transform.x + 20, HEIGHT - 100, 19, 13 } }
		});

		playBGM("sound/SEQ_BG_002.wav");
		m_fading = true;
		Scene::load();
	}

	void StMystereEntry::render() {
		if (m_topBackground == 0) {
			m_sprites.at("topBackground")->draw();
		}

		if (m_bottomBackground == 0 || m_sprites.at("bottomBackground")->m_fading) {
			m_sprites.at("bottomBackground")->draw();
		}

		if (m_bottomBackground == 1 || m_sprites.at("bottomBackground2")->m_fading) {
			m_sprites.at("bottomBackground2")->draw();

			if (m_sprites.at("layton")->m_fading || m_dialogueProgression < 4) {
				m_sprites.at("layton")->draw();
				m_sprites.at("luke")->draw();
			}

			if (m_dialogueProgression == 4) {
				m_sprites.at("layton2")->draw();
				m_sprites.at("luke2")->draw();
			}

			if (!m_sprites.at("bottomBackground2")->m_fading && m_dialogueProgression == 2) {
				m_dialogue.setCharacterVisible("layton2", true);
				m_dialogue.setCharacterVisible("luke", true);
				m_dialogue.setDialogue("etext/fr/e33_t1.txt", "luke", {}, "97.wav");
				m_dialogue.setVisible(true);
				m_dialogueProgression++;
			}
		}

		if (!m_fading && m_dialogueProgression == 0) {
			m_dialogue.setCharacterVisible("layton", true);
			m_dialogue.setDialogue("etext/fr/e33_t0.txt", "layton", {});
			m_dialogue.setVisible(true);
			m_dialogueProgression++;
		}

		m_dialogue.draw();

		Scene::render();
	}

	void StMystereEntry::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_dialogueProgression >= 1) {
			if (m_dialogue.waiting()) {
				if (m_dialogue.next()) {
					return;
				}

				switch (m_dialogueProgression) {
					case 1:
						m_sprites.at("bottomBackground")->fade(Sprite::FadeInfo{ 300, -300, Sprite::FadingMode::OUT });
						m_sprites.at("bottomBackground2")->fade(Sprite::FadeInfo{ 300, -900, Sprite::FadingMode::IN });
						m_sprites.at("luke")->fade(Sprite::FadeInfo{ 50, -900, Sprite::FadingMode::IN });
						m_sprites.at("layton")->fade(Sprite::FadeInfo{ 50, -900, Sprite::FadingMode::IN });
						m_bottomBackground = 1;
						m_dialogue.setVisible(false);
						m_dialogue.setCharacterVisible("layton", false);
						m_dialogueProgression++;
						break;
					case 3:
						m_sprites.at("luke")->fade(Sprite::FadeInfo{ 300, 0, Sprite::FadingMode::OUT });
						m_sprites.at("layton")->fade(Sprite::FadeInfo{ 300, 0, Sprite::FadingMode::OUT });
						m_sprites.at("luke2")->fade(Sprite::FadeInfo{ 300, 0, Sprite::FadingMode::IN });
						m_sprites.at("layton2")->fade(Sprite::FadeInfo{ 300, 0, Sprite::FadingMode::IN });
						m_dialogue.setCharacterVisible("luke", false);
						m_dialogue.setCharacterVisible("luke2", true);
						m_dialogue.setDialogue("etext/fr/e33_t2.txt", "luke", {}, "97.wav");
						m_dialogueProgression++;
						break;
					case 4:
						fadeToNextScene("room1");
						break;
				}
			} else {
				m_dialogue.skip();
			}
		}
	}
};
