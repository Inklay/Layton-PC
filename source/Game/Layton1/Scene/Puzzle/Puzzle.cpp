#include "Game/Layton1/Scene/Puzzle/Puzzle.h"
#include "Game/Game.h"
#include "Game/Sprite/TextSprite.h"

namespace Layton1Scene {
	Puzzle::Puzzle(Game* game, int picarat, int number, const fileUtils::path& nameFilePath) :
		Scene(game, 0.5f, 0.5f),
		m_picarat(picarat),
		m_currentPicarat(picarat),
		m_number(number),
		m_nameFilePath(nameFilePath)
	{
	}

	void Puzzle::load() {
		m_sprites.insert({ "intro_title", std::make_unique<Sprite>("bg/q_alt_sub_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_picaratBackground", std::make_unique<Sprite>("bg/fr/picarat_bg.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_puzzleText", std::make_unique<Sprite>("ani/fr/pazzle_mes.png", this, SDL_FRect{ centerXPos(88), HALF_HEIGHT + 30, 88, 14})});
		m_sprites.insert({ "intro_bottomFading", std::make_unique<Sprite>("bg/custom/black_screen.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "intro_puzzleName", std::make_unique<TextSprite>("font/fontevent.png", m_nameFilePath, this, SDL_FRect{ -1, HALF_HEIGHT + 120, WIDTH, 12 }, SDL_Color{0, 0, 0}) });

		std::vector<fileUtils::path> numberSprites = getNumberSprites(m_currentPicarat, "picarat_number_big");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_currentPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 92.0f - i * 40, HALF_HEIGHT + 55, 36, 58})});
		}

		numberSprites = getNumberSprites(m_picarat, "picarat_get_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_picarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 183.0f - i * 28, HALF_HEIGHT + 77, 22, 36}) });
		}

		numberSprites = getNumberSprites(m_game->m_save->m_picarats, "picarat_number_small");
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_totalPicarat" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 188.0f - i * 16, HEIGHT - 22, 8, 14}) });
		}

		numberSprites = getNumberSprites(m_number, "qintro_numbers", 3);
		for (size_t i = 0; i < numberSprites.size(); i++) {
			m_sprites.insert({ "intro_puzzleNumber" + std::to_string(i), std::make_unique<Sprite>(numberSprites.at(i), this, SDL_FRect{ 175.0f - i * 32, HALF_HEIGHT + 54, 32, 48}) });
		}

		m_fading = true;
		playSFX("puzzleStart");
		m_sprites.at("intro_bottomFading")->fade({ 300, -1000, Sprite::FadingMode::OUT });
		Scene::load();
	}

	void Puzzle::render() {
		if (m_isIntro) {
			if (!m_fading && !m_movedTitleCard) {
				m_sprites.at("intro_title")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleText")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleName")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber0")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber1")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_sprites.at("intro_puzzleNumber2")->translate(Sprite::TranslationInfo{ 400, 0.0f, -HALF_HEIGHT });
				m_movedTitleCard = true;
			}

			if (m_movedTitleCard && !m_sprites.at("intro_title")->m_translating) {
				m_sprites.at("intro_picaratBackground")->draw();

				m_sprites.at("intro_currentPicarat0")->draw();
				m_sprites.at("intro_currentPicarat1")->draw();

				m_sprites.at("intro_picarat0")->draw();
				m_sprites.at("intro_picarat1")->draw();

				m_sprites.at("intro_totalPicarat0")->draw();
				if (m_game->m_save->m_picarats > 10) {
					m_sprites.at("intro_totalPicarat2")->draw();
				}
				if (m_game->m_save->m_picarats > 100) {
					m_sprites.at("intro_totalPicarat3")->draw();
				}
				if (m_game->m_save->m_picarats > 1000) {
					m_sprites.at("intro_totalPicarat4")->draw();
				}
			}

			m_sprites.at("intro_bottomFading")->draw();
			m_sprites.at("intro_title")->draw();
			m_sprites.at("intro_puzzleText")->draw();
			m_sprites.at("intro_puzzleName")->draw();
			m_sprites.at("intro_puzzleNumber0")->draw();
			m_sprites.at("intro_puzzleNumber1")->draw();
			m_sprites.at("intro_puzzleNumber2")->draw();
		}

		Scene::render();
	}

	void Puzzle::handleClick(const std::string& spriteName, SDL_Event event) {

	}

	std::vector<fileUtils::path> Puzzle::getNumberSprites(int number, const std::string& font, int padding) {
		std::vector<fileUtils::path> vec;
		int digit;

		if (number == 0) {
			vec.emplace_back("ani/" + font + ".0.png");
			return vec;
		}

		while (number != 0) {
			digit = number % 10;
			vec.emplace_back("ani/" + font + "." + std::to_string(digit) + ".png");
			number /= 10;
		}

		while (vec.size() < padding) {
			std::cout << vec.size() << std::endl;
			vec.emplace_back("ani/" + font + ".0.png");
		}

		return vec;
	}
};
