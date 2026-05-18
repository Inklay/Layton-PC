#include "Game/layton1/Scene/map/DrawBridge.h"
#include "Game/Game.h"
#include "Game/Sprite/AnimatedSprite.h"

namespace Layton1Scene {
	DrawBridge::DrawBridge(Game* game) :
		Map(game, "sound/SEQ_BG_002.wav", "1")
	{}

	void DrawBridge::loadMap() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/map_1.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackground", std::make_unique<Sprite>("bg/room_1_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "anim1", std::make_unique<AnimatedSprite>("ani/r1_bgobj_1.gfx.anim", this, SDL_FRect{ 158, HALF_HEIGHT, 6, 30 }, true) });
		m_sprites.insert({ "anim2", std::make_unique<AnimatedSprite>("ani/r1_bgobj_2.gfx.anim", this, SDL_FRect{ 65, HEIGHT - 28, 32, 32 }, true) });
		m_sprites.insert({ "anim3", std::make_unique<AnimatedSprite>("ani/r1_bgobj_3.gfx.anim", this, SDL_FRect{ 129, HEIGHT - 28, 16, 32 }, true) });

		if (m_game->m_save->m_chapter == 0) {
			m_sprites.insert({ "car", std::make_unique<Sprite>("ani/obj_2.png", this, SDL_FRect{ 49, HALF_HEIGHT + 109, 48, 40 }, true) });
			m_sprites.insert({ "laytonAndLuke", std::make_unique<AnimatedSprite>("ani/obj_1.gfx.anim", this, SDL_FRect{ 20, HALF_HEIGHT + 111, 30, 48 }, true) });
			m_sprites.insert({ "franco", std::make_unique<AnimatedSprite>("ani/obj_3.gfx.anim", this, SDL_FRect{ 158, HALF_HEIGHT + 109, 26, 42 }, true) });

			m_dialogues = {
				{ "laytonAndLuke", 2 },
				{ "car", 3 }
			};
		}
	}

	void DrawBridge::loadDialogue() {
		m_sprites.insert({ "topBackgroundDialogue1", std::make_unique<Sprite>("bg/ebgs_1.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "bottomBackgroundDialogue1", std::make_unique<Sprite>("bg/ara_1_1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });

		m_sprites.insert({ "layton", std::make_unique<Sprite>("ani/layton.5.png", this, SDL_FRect{ 20, HEIGHT - 176, 105, 176}, true) });
		m_sprites.insert({ "layton2", std::make_unique<Sprite>("ani/layton.2.png", this, SDL_FRect{ 20, HEIGHT - 176, 105, 176}, true) });
		m_sprites.insert({ "luke", std::make_unique<Sprite>("ani/luke.1.png", this, SDL_FRect{ 160, HEIGHT - 142, 68, 142 }, true) });
		m_sprites.insert({ "luke2", std::make_unique<Sprite>("ani/luke.3.png", this, SDL_FRect{ 160, HEIGHT - 142, 68, 142 }, true) });

		m_dialogue.load({
			{ "layton", "ani/fr/layton_n.png", "ani/event_window_1.1.png", "", "", SDL_FRect{ 0, 0, 0, 0 } },
			{ "layton2", "ani/fr/luke_n.png", "ani/event_window_1.2.png", "ani/layton_e_face.notalk.anim", "ani/layton_e_face.talk.anim", SDL_FRect{ (m_sprites.at("layton")->m_transform.x / m_game->m_windowMultiplier) + 38, HEIGHT - 119, 21, 13 } },
			{ "luke", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_e_face.notalk.anim", "ani/luke_e_face.talk.anim", SDL_FRect{ (m_sprites.at("luke")->m_transform.x / m_game->m_windowMultiplier) + 17, HEIGHT - 99, 19, 13 } },
			{ "luke2", "ani/fr/luke_n.png", "ani/event_window_1.1.png", "ani/luke_e_face.notalk.anim", "ani/luke_e_face.smile.anim", SDL_FRect{ (m_sprites.at("luke")->m_transform.x / m_game->m_windowMultiplier) + 17, HEIGHT - 99, 19, 13 } }
		});

		if (m_game->m_save->m_storyProgression == 0) {
			m_nextDialogueId = 0;
			m_currentDialogueId = 0;
		}
	}

	void DrawBridge::renderMap() {
		m_sprites.at("bottomBackground")->draw();
		m_sprites.at("topBackground")->draw();
		m_sprites.at("anim1")->draw();
		m_sprites.at("anim2")->draw();
		m_sprites.at("anim3")->draw();

		if (m_game->m_save->m_chapter == 0) {
			m_sprites.at("car")->draw(1);
			m_sprites.at("laytonAndLuke")->draw(1);
			m_sprites.at("franco")->draw(1);
		}
	}

	void DrawBridge::renderDialogue() {
		if (m_currentDialogueId == 0) {
			m_sprites.at("topBackgroundDialogue1")->draw();
			m_sprites.at("bottomBackgroundDialogue1")->draw();

			if (!m_fading && m_dialogueProgression == 0) {
				m_dialogue.setCharacterVisible("layton", true);
				m_dialogue.setDialogue("etext/fr/e33_t0.txt", "layton", {});
				m_dialogue.setVisible(true);
				m_dialogueProgression++;
			}
		} else if (m_currentDialogueId == 1) {
			m_sprites.at("topBackgroundDialogue1")->draw();
			m_sprites.at("bottomBackground")->draw();

			if (m_dialogueProgression < 2) {
				m_sprites.at("layton")->draw();
				m_sprites.at("luke")->draw();
			} else {
				m_sprites.at("layton2")->draw();
				m_sprites.at("luke2")->draw();
			}

			if (!isFadingToDialogue() && m_dialogueProgression == 0) {
				m_dialogue.setCharacterVisible("luke", true);
				m_dialogue.setDialogue("etext/fr/e33_t1.txt", "luke", {}, "charBip1");
				m_dialogue.setVisible(true);
				m_dialogueProgression++;
			}
		} else if (m_currentDialogueId == 2) {
			m_sprites.at("topBackgroundDialogue1")->draw();
			m_sprites.at("bottomBackground")->draw();
			m_sprites.at("layton")->draw();

			if (!isFadingToDialogue() && m_dialogueProgression == 0) {
				m_dialogue.setCharacterVisible("layton2", true);
				m_dialogue.setDialogue("etext/fr/e2_t0.txt", "layton2", {}, "charBip3");
				m_dialogue.setVisible(true);
				m_dialogueProgression++;
			}
		} else if (m_currentDialogueId == 3) {
			m_sprites.at("topBackgroundDialogue1")->draw();
			m_sprites.at("bottomBackground")->draw();
			m_sprites.at("layton")->draw();
			m_sprites.at("luke")->draw();

			if (!isFadingToDialogue() && m_dialogueProgression == 0) {
				m_dialogue.setCharacterVisible("luke", true);
				m_dialogue.setCharacterVisible("layton2", true);
				m_dialogue.setDialogue("etext/fr/e3_t0.txt", "luke", {}, "charBip1");
				m_dialogue.setVisible(true);
				m_dialogueProgression++;
			}
		}
	}

	void DrawBridge::handleClick(const std::string& spriteName, SDL_Event event) {
		Map::handleClick(spriteName, event);
	}

	void DrawBridge::handleDialogProgression() {
		if (m_currentDialogueId == 0) {
			m_dialogue.setCharacterVisible("layton", false);
			changeDialogue(1);
		} else if (m_currentDialogueId == 1) {
			switch (m_dialogueProgression) {
				case 1:
					m_dialogue.setCharacterVisible("luke", false);
					m_dialogue.setCharacterVisible("luke2", true);
					m_dialogue.setDialogue("etext/fr/e33_t2.txt", "luke2", {}, "charBip1");
					break;
				case 2:
					changeDialogue(-1);
					m_game->m_save->m_storyProgression++;
					break;
			}
		} else if (m_currentDialogueId == 2 && m_dialogueProgression == 1) {
			m_dialogue.setCharacterVisible("layton2", false);
			changeDialogue(-1);
		} else if (m_currentDialogueId == 3) {
			switch (m_dialogueProgression) {
				case 1:
					m_dialogue.setDialogue("etext/fr/e3_t1.txt", "layton2", {}, "charBip3");
					break;
				case 2:
					changeDialogue(-1);
					break;
			}
		}
	}
};
