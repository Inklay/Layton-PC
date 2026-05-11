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
		if (!c.talkAnim.empty()) {
			m_scene->m_sprites.insert({ c.talkAnim.string(), std::make_unique<AnimatedSprite>(c.talkAnim, m_scene, c.rect, true) });
			m_scene->m_sprites.insert({ c.noTalkAnim.string(), std::make_unique<AnimatedSprite>(c.noTalkAnim, m_scene, c.rect, true) });
		}
		m_scene->m_sprites.insert({ c.nameSprite.string(), std::make_unique<Sprite>(c.nameSprite, m_scene, SDL_FRect{ 3, HEIGHT - 68, 62, 14 }, true) });

		if (m_scene->m_sprites.count(c.backgroundSprite.string()) == 0) {
			m_scene->m_sprites.insert({ c.backgroundSprite.string(), std::make_unique<Sprite>(c.backgroundSprite, m_scene, SDL_FRect{ 2, HEIGHT - 66, 252, 64 }, true) });
		}
	}

	m_scene->m_sprites.insert({ "dialogue_cursor", std::make_unique<AnimatedSprite>("ani/cursor_wait.touch.anim", m_scene, SDL_FRect{ WIDTH - 23, HEIGHT - 20, 16, 16 }, true) });
	m_scene->m_sprites.insert({ "dialogue_text", std::make_unique<TextSprite>("font/fontevent.png", std::u32string(U""), m_scene, SDL_FRect{ 14, HEIGHT - 51, 234, 60 }, SDL_Color{0, 0, 0}) });
	m_audioStreamIdx = m_scene->m_game->m_bgmStreams.size();
}

void Dialogue::setDialogue(const fileUtils::path& textFilePath, const std::string& character, std::vector<fileUtils::path> audioFilesPath) {
	for (auto& c : m_characters) {
		if (c.talking && c.name != character) {
			c.talking = false;
		}

		if (c.name == character) {
			c.talking = true;
			m_offscreen = c.talkAnim.empty();
		}
	}

	std::u32string texts = fileUtils::readText(m_scene->m_game->m_gameFolder / textFilePath);
	std::u32string::size_type pos = texts.find(U"@p@c\n");
	m_texts.clear();

	while (pos != std::u32string::npos) {
		m_texts.emplace_back(texts.substr(0, pos));
		texts = texts.substr(pos + 5);
		pos = texts.find(U"@p@c\n");
	}
	m_texts.emplace_back(texts);

	m_textProgression = 0;
	m_currentText = 0;
	m_delayCounter = 0;
	m_waiting = false;
	m_audioFilesPath = audioFilesPath;

	if (!m_audioFilesPath.empty()) {
		if (m_scene->m_game->m_bgmStreams.size() > m_audioStreamIdx) {
			clearAudioStream(m_scene->m_game->m_bgmStreams.at(m_audioStreamIdx), m_scene->m_game->m_bgmData.at(m_audioStreamIdx).get());
		}
		m_scene->playBGM(m_audioFilesPath.at(m_currentText), m_audioStreamIdx, false);
	}
}

void Dialogue::draw() {
	for (auto& c : m_characters) {
		if (c.talking) {
			if (m_displayed) {
				m_scene->m_sprites.at(c.backgroundSprite.string())->draw();
				m_scene->m_sprites.at(c.nameSprite.string())->draw();

				if (m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->finished && !m_offscreen) {
					m_scene->m_sprites.at(c.noTalkAnim.string())->draw();
					continue;
				}
			}

			if (!m_offscreen) {
				m_scene->m_sprites.at(c.talkAnim.string())->draw();
			}
		} else if (!c.noTalkAnim.empty() && !m_offscreen) {
			m_scene->m_sprites.at(c.noTalkAnim.string())->draw();
		}
	}

	if (m_displayed) {
		m_scene->m_sprites.at("dialogue_text")->draw();
		m_delayCounter++;

		if (m_delayCounter == 2 && m_textProgression <= m_texts.at(m_currentText).length()) {
			m_delayCounter = 0;
			m_scene->m_sprites.at("dialogue_text")->setText(m_texts.at(m_currentText).substr(0, m_textProgression));
			m_textProgression++;
		} else if (m_textProgression > m_texts.at(m_currentText).length()) {
			m_scene->m_sprites.at("dialogue_cursor")->draw();
			m_waiting = true;
		}
	}
}

void Dialogue::setVisible(bool visible) {
	m_displayed = visible;
}

bool Dialogue::waiting() const {
	return m_waiting;
}

void Dialogue::skip() {
	m_textProgression = m_texts.at(m_currentText).length();
	m_waiting = true;
}

bool Dialogue::next() {
	if (m_currentText < m_texts.size() - 1) {
		m_currentText++;
		m_textProgression = 0;
		m_delayCounter = 0;
		m_waiting = false;

		if (!m_audioFilesPath.empty()) {
			if (m_scene->m_game->m_bgmStreams.size() > m_audioStreamIdx) {
				clearAudioStream(m_scene->m_game->m_bgmStreams.at(m_audioStreamIdx), m_scene->m_game->m_bgmData.at(m_audioStreamIdx).get());
			}
			m_scene->playBGM(m_audioFilesPath.at(m_currentText), m_audioStreamIdx, false);
		}

		return true;
	}
	return false;
}
