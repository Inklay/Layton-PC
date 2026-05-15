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
		m_game->m_save->m_chapterProgression = 1;
		m_game->m_save->m_solvedPuzzles = 1;
#endif
		m_sprites.insert({ "bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "fading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, 0, WIDTH, HEIGHT }) });

		m_sprites.insert({ "solved", std::make_unique<Sprite>("ani/fr/puzzle_numbers.10.png", this, SDL_FRect{ 2, 3, 52, 22 }) });
		m_sprites.insert({ "name", std::make_unique<TextSprite>("font/fontevent.png", "storytext/fr/map" + m_internalName + ".txt", this, SDL_FRect{ -85, 7, WIDTH, 12 }, SDL_Color{ 0, 0, 0 }) });

		std::vector<fileUtils::path> numberSprites = getNumberSprites(m_game->m_save->m_solvedPuzzles, "fr/puzzle_numbers");
		for (size_t i = numberSprites.size() - 1; i != SIZE_MAX; i--) {
			m_sprites.insert({ "solvedNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 58.0f + i * 10, 8, 8, 12}) });
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
