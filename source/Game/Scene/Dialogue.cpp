#include "Game/Scene/Dialogue.h"
#include "Game/Scene/Scene.h"
#include "Game/Game.h"
#include "Game/Sprite/AnimatedSprite.h"
#include "Game/Sprite/TextSprite.h"

Dialogue::Dialogue(Scene* scene) :
	m_scene(scene)
{
}

void Dialogue::load(std::vector<Character> characters) {
	m_characters = characters;

	for (const auto& c : characters) {
		m_scene->m_sprites.insert({ c.talkAnim.string(), std::make_unique<AnimatedSprite>(c.talkAnim, m_scene, c.rect)});
		m_scene->m_sprites.insert({ c.noTalkAnim.string(), std::make_unique<AnimatedSprite>(c.noTalkAnim, m_scene, c.rect) });
		m_scene->m_sprites.insert({ c.nameSprite.string(), std::make_unique<Sprite>(c.nameSprite, m_scene, SDL_FRect{ 3, HEIGHT - 68, 62, 14 }) });

		if (m_scene->m_sprites.count(c.backgroundSprite.string()) == 0) {
			m_scene->m_sprites.insert({ c.backgroundSprite.string(), std::make_unique<Sprite>(c.backgroundSprite, m_scene, SDL_FRect{ 2, HEIGHT - 66, 252, 64 }) });
		}
	}

	m_scene->m_sprites.insert({ "dialogue_cursor", std::make_unique<AnimatedSprite>("ani/cursor_wait.touch.anim", m_scene, SDL_FRect{ WIDTH - 23, HEIGHT - 20, 16, 16 }) });
	m_scene->m_sprites.insert({ "dialogue_text", std::make_unique<TextSprite>("font/fontevent.png", std::u32string(U""), m_scene, SDL_FRect{ 14, HEIGHT - 51, 234, 60 }, SDL_Color{0, 0, 0}) });
}

void Dialogue::setDialogue(const fileUtils::path& textFilePath, const std::string& character, const fileUtils::path& audioFilePath) {
	for (auto& c : m_characters) {
		if (c.talking && c.name != character) {
			c.talking = false;
		}

		if (c.name == character) {
			c.talking = true;
		}
	}

	m_text = fileUtils::readText(m_scene->m_game->m_gameFolder / textFilePath);
	m_textProgression = 0;
	m_delayCounter = 0;
}

void Dialogue::draw() {
	for (const auto& c : m_characters) {
		if (c.talking) {
			m_scene->m_sprites.at(c.talkAnim.string())->draw();

			if (m_displayed) {
				m_scene->m_sprites.at(c.backgroundSprite.string())->draw();
				m_scene->m_sprites.at(c.nameSprite.string())->draw();
			}
		} else {
			m_scene->m_sprites.at(c.noTalkAnim.string())->draw();
		}
	}

	if (m_displayed) {
		m_scene->m_sprites.at("dialogue_text")->draw();
		m_scene->m_sprites.at("dialogue_cursor")->draw();
		m_delayCounter++;

		if (m_delayCounter == 2 && m_textProgression <= m_text.length()) {
			m_delayCounter = 0;
			m_scene->m_sprites.at("dialogue_text")->setText(m_text.substr(0, m_textProgression));
			m_textProgression++;
		}
	}
}

void Dialogue::setVisible(bool visible) {
	m_displayed = visible;
}
