#include "Game/Layton1/Scene/CreateSave.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	CreateSave::CreateSave(Game* game) :
		Scene(game)
	{
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/name_sub.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "keyboardLowerCase", std::make_unique<Sprite>("bg/name_bg1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "keyboardUpperCase", std::make_unique<Sprite>("bg/name_bg2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "keyboardShift", std::make_unique<Sprite>("bg/name_bg3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "keyboardSpecial", std::make_unique<Sprite>("bg/name_bg4.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });

		m_sprites.insert({ "okButton", std::make_unique<ClickableSprite>("ani/fr/name_ok.png", this, SDL_FRect{ centerXPos(64), 353, 64, 24}) });
		m_sprites.insert({ "backSpaceButton", std::make_unique<ClickableSprite>("ani/name_btn.5.png", this, SDL_FRect{ 194, 321, 60, 20}) });
		m_sprites.insert({ "spaceButton", std::make_unique<ClickableSprite>("ani/name_btn.4.png", this, SDL_FRect{ 68, 321, 120, 20}) });
		m_sprites.insert({ "specialButton", std::make_unique<ClickableSprite>("ani/name_btn.3.png", this, SDL_FRect{ 34, 321, 28, 20}) });
		m_sprites.insert({ "lowerButton", std::make_unique<ClickableSprite>("ani/name_btn.2.png", this, SDL_FRect{ 3, 321, 28, 20}) });

		m_sprites.insert({ "cursor", std::make_unique<Sprite>("ani/name_btn.0.png", this, SDL_FRect{ 52, 221, 10, 2})});
	}

	void CreateSave::load() {
		Scene::load();
	}

	void CreateSave::render() {
		loopBGM();

		m_sprites.at("topBackground")->draw();

		switch (m_keyboardState) {
			case LOWER:
				m_sprites.at("keyboardLowerCase")->draw();
				break;
			case UPPER:
				m_sprites.at("keyboardUpperCase")->draw();
				break;
			case SHIFT:
				m_sprites.at("keyboardShift")->draw();
				break;
			case SPECIAL:
				m_sprites.at("keyboardSpecial")->draw();
				break;
		}

		m_sprites.at("okButton")->draw();
		m_sprites.at("backSpaceButton")->draw();
		m_sprites.at("spaceButton")->draw();
		m_sprites.at("specialButton")->draw();
		m_sprites.at("lowerButton")->draw();

		if (m_displayCursor) {
			m_sprites.at("cursor")->draw();
		}

		m_cursorCount++;

		if (m_cursorCount == 30) {
			m_cursorCount = 0;
			m_displayCursor = !m_displayCursor;
		}

		m_lastTick = SDL_GetTicks();
	}

	void CreateSave::handleClick(const std::string& spriteName, SDL_Event event) {
		if (spriteName == "specialButton") {
			m_keyboardState = SPECIAL;
		} else if (spriteName == "lowerButton") {
			m_keyboardState = LOWER;
		} else if (spriteName.starts_with("keyboard")) {
			std::cout << isLetterClicked(15, HALF_HEIGHT + 49) << std::endl;
		}
	}

	bool CreateSave::isLetterClicked(float x, float y) {
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		x *= m_game->m_windowMultiplier;
		y *= m_game->m_windowMultiplier;

		return mouseX >= x && mouseX < x + 18 * m_game->m_windowMultiplier && mouseY >= y && mouseY < y + 18 * m_game->m_windowMultiplier;
	}
};
