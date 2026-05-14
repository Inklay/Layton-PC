#include "Game/Layton1/Scene/Map/Map.h"

namespace Layton1Scene {
	Map::Map(Game* game, const std::string& bgm) :
		Scene(game),
		m_bgm(bgm)
	{
	}

	void Map::load() {
		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }) });
		
		loadMap();
		loadDialogue();

		m_sprites.at("fading")->m_opacity = 0;
		m_sprites.at("bottomFading")->m_opacity = 0;
		m_fading = true;
		playBGM(m_bgm);
		Scene::load();
	}

	void Map::render() {
		if (m_currentDialogueId != m_nextDialogueId && !isFadingToDialogue()) {
			if (m_nextDialogueId == -1 || m_currentDialogueId == -1) {
				m_sprites.at("fading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			} else {
				m_sprites.at("bottomFading")->fade({ 300, 0, Sprite::FadingMode::OUT });
			}

			m_currentDialogueId = m_nextDialogueId;
			m_dialogueProgression = 0;
		}

		if (m_currentDialogueId == -1) {
			renderMap();
		} else {
			renderDialogue();
			m_dialogue.draw();
		}

		m_sprites.at("fading")->draw();
		m_sprites.at("bottomFading")->draw();

		Scene::render();
	}

	void Map::handleClick(const std::string& spriteName, SDL_Event event) {
		if (m_currentDialogueId != -1 && m_dialogueProgression >= 1 && m_currentDialogueId == m_nextDialogueId) {
			if (m_dialogue.waiting()) {
				if (m_dialogue.next()) {
					return;
				}

				handleDialogProgression();
				m_dialogueProgression++;
			} else {
				m_dialogue.skip();
			}
		}
	}

	void Map::changeDialogue(int dialogueId) {
		if (dialogueId == -1 || m_currentDialogueId == -1) {
			m_sprites.at("fading")->fade({ 300, -200, Sprite::FadingMode::IN });
		} else {
			m_sprites.at("bottomFading")->fade({ 300, -200, Sprite::FadingMode::IN });
		}

		m_dialogue.setVisible(false);
		m_nextDialogueId = dialogueId;
	}

	void Map::findHintCoin() {

	}

	bool Map::isFadingToDialogue() {
		return m_sprites.at("fading")->m_fading || m_sprites.at("bottomFading")->m_fading;
	}
};
