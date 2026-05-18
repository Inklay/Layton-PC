#include "Game/Layton1/Scene/Map/Map.h"
#include "Game/Game.h"
#include "Game/Sprite/TextSprite.h"

namespace Layton1Scene {
	Map::Map(Game* game, const std::string& bgm, const std::string& internalName) :
		Scene(game),
		m_bgm(bgm),
		m_internalName(internalName)
	{
	}

	void Map::load() {
#ifdef _DEBUG
		m_game->m_save->m_storyProgression = 1;
		m_game->m_save->m_solvedPuzzles = 1;
#endif
		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }) });

		m_sprites.insert({ "solved", std::make_unique<Sprite>("ani/fr/puzzle_numbers.10.png", this, SDL_FRect{ 2, 3, 52, 22 }) });
		m_sprites.insert({ "name", std::make_unique<TextSprite>("font/fontevent.png", "storytext/fr/map" + m_internalName + ".txt", this, SDL_FRect{ -85, 7, WIDTH, 12 }, SDL_Color{ 0, 0, 0, 255 }) });
		
		m_questText = m_game->getQuestText();
		if (m_questText.length() > 0) {
			m_sprites.insert({ "quest", std::make_unique<TextSprite>("font/fontevent.png", m_questText, this, SDL_FRect{ -1, HALF_HEIGHT - 20, WIDTH, 12 }, SDL_Color{ 0, 0, 0, 255 }) });
		}

		std::vector<fileUtils::path> numberSprites = getNumberSprites(m_game->m_save->m_solvedPuzzles, "fr/puzzle_numbers");
		for (size_t i = numberSprites.size() - 1; i != SIZE_MAX; i--) {
			m_sprites.insert({ "solvedNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 56.0f + i * 10, 8, 8, 12}) });
		}
		
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

			m_sprites.at("name")->draw();
			m_sprites.at("solved")->draw();
			m_sprites.at("solvedNumber0")->draw();
			
			if (m_questText.length() > 0) {
				m_sprites.at("quest")->draw();
			}

			if (m_sprites.count("solvedNumber1")) {
				m_sprites.at("solvedNumber1")->draw();
			}
			if (m_sprites.count("solvedNumber2")) {
				m_sprites.at("solvedNumber2")->draw();
			}
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
		} else {
			for (const auto& dialogue : m_dialogues) {
				if (spriteName == dialogue.first) {
					changeDialogue(dialogue.second);
					return;
				}
			}

			SDL_FPoint coord = { event.button.x / m_game->m_windowMultiplier, event.button.y / m_game->m_windowMultiplier };

			for (const auto& dialogue : m_hints) {
				if (coord.x >= dialogue.first.x && coord.x < dialogue.first.x + dialogue.first.w && coord.y >= dialogue.first.y && coord.y < dialogue.first.y + dialogue.first.h) {
					//changeDialogue(dialogue.second);
					return;
				}
			}

			for (const auto& dialogue : m_puzzles) {
				if (coord.x >= dialogue.first.x && coord.x < dialogue.first.x + dialogue.first.w && coord.y >= dialogue.first.y && coord.y < dialogue.first.y + dialogue.first.h) {
					//changeDialogue(dialogue.second);
					return;
				}
			}
		}
	}

	void Map::changeDialogue(int dialogueId) {
		if (dialogueId == -1 || m_currentDialogueId == -1) {
			m_sprites.at("fading")->fade({ 300, -200, Sprite::FadingMode::IN });

			if (dialogueId == -1) {
				m_dialogue.hideAllCharacters();
			}
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

	void Map::unload() {
		m_dialogues.clear();
		m_hints.clear();
		m_puzzles.clear();

		Scene::unload();
	}

};
