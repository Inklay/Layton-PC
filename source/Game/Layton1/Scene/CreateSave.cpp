#include "Game/Layton1/Scene/CreateSave.h"
#include "Game/Sprite/ParallaxSprite.h"
#include "Game/Sprite/FadingSprite.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/ClickableSprite.h"
#include "Game/Sprite/TextSprite.h"
#include "Game/Game.h"

namespace Layton1Scene {
	CreateSave::CreateSave(Game* game) :
		Scene(game)
	{
	}

	void CreateSave::load() {
		m_sprites.insert({ "topBackground", std::make_unique<Sprite>("bg/fr/name_sub.png", this, SDL_FRect{ 0, 0, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "keyboardLowerCase", std::make_unique<Sprite>("bg/name_bg1.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }, true) });
		m_sprites.insert({ "keyboardUpperCase", std::make_unique<Sprite>("bg/name_bg2.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "keyboardShift", std::make_unique<Sprite>("bg/name_bg3.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });
		m_sprites.insert({ "keyboardSpecial", std::make_unique<Sprite>("bg/name_bg4.png", this, SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT }) });

		m_sprites.insert({ "okButton", std::make_unique<ClickableSprite>("ani/fr/name_ok.png", this, SDL_FRect{ centerXPos(64), 353, 64, 24}) });
		m_sprites.insert({ "backSpaceButton", std::make_unique<ClickableSprite>("ani/name_btn.5.png", this, SDL_FRect{ 194, 321, 60, 20}) });
		m_sprites.insert({ "spaceButton", std::make_unique<ClickableSprite>("ani/name_btn.4.png", this, SDL_FRect{ 68, 321, 120, 20}) });
		m_sprites.insert({ "specialButton", std::make_unique<ClickableSprite>("ani/name_btn.3.png", this, SDL_FRect{ 34, 321, 28, 20}) });
		m_sprites.insert({ "lowerButton", std::make_unique<ClickableSprite>("ani/name_btn.2.png", this, SDL_FRect{ 3, 321, 28, 20}) });

		m_sprites.insert({ "cursor", std::make_unique<Sprite>("ani/name_btn.0.png", this, SDL_FRect{ 52, 221, 10, 2}) });
		m_sprites.insert({ "char", std::make_unique<TextSprite>("font/fontevent.png", this, SDL_FRect{ 53, 210, 9, 12}) });

		m_fading = true;
		m_fadeInSeconds = 0.8f;
		m_fadeOutSeconds = 0.3f;
	}

	void CreateSave::render() {
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

		m_sprites.at("okButton")->draw(1);
		m_sprites.at("backSpaceButton")->draw(1);
		m_sprites.at("spaceButton")->draw(1);
		m_sprites.at("specialButton")->draw(1);
		m_sprites.at("lowerButton")->draw(1);

		if (m_displayCursor && m_name.length() != 10) {
			m_sprites.at("cursor")->draw();
		}

		m_sprites.at("char")->m_transform.x = 53 * m_game->m_windowMultiplier;
		for (size_t i = 0; i < m_name.length(); i++) {
			m_sprites.at("char")->drawText(m_name.substr(i, 1));
			m_sprites.at("char")->m_transform.x += 16 * m_game->m_windowMultiplier;
		}

		m_cursorCount++;
		if (m_cursorCount == 30) {
			m_cursorCount = 0;
			m_displayCursor = !m_displayCursor;
		}

		fade();
		m_lastTick = SDL_GetTicks();
	}

	void CreateSave::handleClick(const std::string& spriteName, SDL_Event event) {
		if (spriteName == "specialButton") {
			playSFX("keyboardSwitch");
			m_keyboardState = SPECIAL;
			m_sprites.at("keyboardLowerCase")->m_interactive = false;
			m_sprites.at("keyboardUpperCase")->m_interactive = false;
			m_sprites.at("keyboardShift")->m_interactive = false;
			m_sprites.at("keyboardSpecial")->m_interactive = true;
		} else if (spriteName == "lowerButton") {
			playSFX("keyboardSwitch");
			m_keyboardState = LOWER;
			m_sprites.at("keyboardSpecial")->m_interactive = false;
			m_sprites.at("keyboardLowerCase")->m_interactive = true;
		} else if (spriteName == "keyboardLowerCase") {
			checkKeyPressed(m_lowerCaseKeyPos);
		} else if (spriteName == "keyboardUpperCase") {
			checkKeyPressed(m_upperCaseKeyPos);
		} else if (spriteName == "keyboardShift") {
			checkKeyPressed(m_shiftKeyPos);
		} else if (spriteName == "keyboardSpecial") {
			checkKeyPressed(m_specialKeyPos);
		} else if (spriteName == "backSpaceButton") {
			playSFX("keyboardBackspace");
			if (m_name.length() != 0) {
				m_name.pop_back();
				m_sprites.at("cursor")->m_transform.x -= 16 * m_game->m_windowMultiplier;
			}
		} else if (spriteName == "spaceButton") {
			addChar(U" ");
		} else if (spriteName == "okButton" && m_name.length() != 0) {
			playSFX("keyboardOk");
			m_game->addSave(m_name);
			fadeToNextScene(INTRO);
		}
	}

	bool CreateSave::isLetterClicked(SDL_FRect rect) {
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		rect.x *= m_game->m_windowMultiplier;
		rect.y *= m_game->m_windowMultiplier;
		rect.h *= m_game->m_windowMultiplier;
		rect.w *= m_game->m_windowMultiplier;

		return mouseX >= rect.x && mouseX < rect.x + rect.w && mouseY >= rect.y && mouseY < rect.y + rect.h;
	}

	void CreateSave::checkKeyPressed(const std::map<std::u32string, std::pair<float, float>>& keyLayout) {
		for (auto& key : keyLayout) {
			if (key.first == U"upper" && isLetterClicked(SDL_FRect{ key.second.first, HALF_HEIGHT + key.second.second, 25, 12 })) {
				playSFX("keyboardSwitch");
				if (m_keyboardState == UPPER) {
					m_keyboardState = LOWER;
					m_sprites.at("keyboardUpperCase")->m_interactive = false;
					m_sprites.at("keyboardShift")->m_interactive = false;
					m_sprites.at("keyboardLowerCase")->m_interactive = true;
				} else {
					m_keyboardState = UPPER;
					m_sprites.at("keyboardShift")->m_interactive = false;
					m_sprites.at("keyboardLowerCase")->m_interactive = false;
					m_sprites.at("keyboardUpperCase")->m_interactive = true;
				}

				return;
			}

			if (key.first == U"shift" && isLetterClicked(SDL_FRect{ key.second.first, HALF_HEIGHT + key.second.second, 36, 12 })) {
				playSFX("keyboardSwitch");
				m_keyboardState = SHIFT;
				m_sprites.at("keyboardLowerCase")->m_interactive = false;
				m_sprites.at("keyboardUpperCase")->m_interactive = false;
				m_sprites.at("keyboardShift")->m_interactive = true;

				return;
			}

			if (isLetterClicked(SDL_FRect{ key.second.first, HALF_HEIGHT + key.second.second, 17, 17 })) {
				if (m_keyboardState == SHIFT) {
					m_keyboardState = LOWER;
					m_sprites.at("keyboardShift")->m_interactive = false;
					m_sprites.at("keyboardLowerCase")->m_interactive = true;
				}
				addChar(key.first);
			}
		}
	}

	void CreateSave::addChar(const std::u32string& c) {
		playSFX("keyboardChar");
		if (m_name.length() == 10 || c.length() > 1) {
			return;
		}

		m_name += c;
		m_sprites.at("cursor")->m_transform.x += 16 * m_game->m_windowMultiplier;
	}
};
