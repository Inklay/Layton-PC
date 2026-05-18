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
			m_scene->m_sprites.insert({ c.talkAnim.string(), std::make_unique<AnimatedSprite>(c.talkAnim, m_scene, c.rect) });
			m_scene->m_sprites.insert({ c.noTalkAnim.string(), std::make_unique<AnimatedSprite>(c.noTalkAnim, m_scene, c.rect) });
		}
		m_scene->m_sprites.insert({ c.nameSprite.string(), std::make_unique<Sprite>(c.nameSprite, m_scene, SDL_FRect{ 3, HEIGHT - 68, 62, 14 }) });

		if (m_scene->m_sprites.count(c.backgroundSprite.string()) == 0) {
			m_scene->m_sprites.insert({ c.backgroundSprite.string(), std::make_unique<Sprite>(c.backgroundSprite, m_scene, SDL_FRect{ 2, HEIGHT - 66, 252, 64 }) });
		}
	}

	m_scene->m_sprites.insert({ "dialogue_cursor", std::make_unique<AnimatedSprite>("ani/cursor_wait.touch.anim", m_scene, SDL_FRect{ WIDTH - 23, HEIGHT - 20, 16, 16 }) });
	m_scene->m_sprites.insert({ "dialogue_text", std::make_unique<TextSprite>("font/fontevent.png", std::u32string(U""), m_scene, SDL_FRect{ 14, HEIGHT - 51, 234, 60 }, SDL_Color{ 0, 0, 0, 255 }) });
	m_audioStreamIdx = m_scene->m_game->m_bgmStreams.size();
}

void Dialogue::setDialogue(const fileUtils::path& textFilePath, const std::string& character, std::vector<fileUtils::path> audioFilesPath, const std::string& sfxSound) {
	for (auto& c : m_characters) {
		if (c.talking && c.name != character) {
			c.talking = false;
		}

		if (c.name == character) {
			c.talking = true;
		}

		if (!c.talkAnim.empty()) {
			m_scene->m_sprites.at(c.talkAnim.string())->m_interactive = true;
			m_scene->m_sprites.at(c.noTalkAnim.string())->m_interactive = true;
		}

		m_scene->m_sprites.at(c.nameSprite.string())->m_interactive = true;
		m_scene->m_sprites.at(c.backgroundSprite.string())->m_interactive = true;
	}
	m_scene->m_sprites.at("dialogue_cursor")->m_interactive = true;

	std::u32string texts = fileUtils::readText(m_scene->m_game->m_gameFolder / textFilePath);
	std::u32string::size_type pos = texts.find(U"@p");
	m_texts.clear();

	while (pos != std::u32string::npos) {
		m_texts.emplace_back(texts.substr(0, pos));
		texts = texts.substr(pos + 2);
		pos = texts.find(U"@p");
	}

	m_texts.emplace_back(texts);

	m_sfxSound = sfxSound;
	m_textProgression = 0;
	m_currentText = 0;
	m_delayCounter = 0;
	m_waiting = false;
	m_audioFilesPath = audioFilesPath;
	m_writtenText = U"";
	m_scene->m_sprites.at("dialogue_text")->setText(m_writtenText);

	if (m_scene->m_game->m_bgmStreams.size() > m_audioStreamIdx) {
		clearAudioStream(m_scene->m_game->m_bgmStreams.at(m_audioStreamIdx), m_scene->m_game->m_bgmData.at(m_audioStreamIdx).get());
	}
}

void Dialogue::draw() {
	for (auto& c : m_characters) {
		if (c.talking) {
			if (m_displayed || fading()) {
				m_scene->m_sprites.at(c.backgroundSprite.string())->draw();
				m_scene->m_sprites.at(c.nameSprite.string())->draw();

				if (m_scene->m_game->m_bgmData.size() > m_audioStreamIdx && ((m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->finished && m_audioFilesPath.size() > 0) || (m_textProgression >= m_texts.at(m_currentText).length() && m_audioFilesPath.size() == 0)) && c.visible && !c.noTalkAnim.empty()) {
					m_scene->m_sprites.at(c.noTalkAnim.string())->draw();
					continue;
				}
			}

			if (c.visible && !c.talkAnim.empty()) {
				m_scene->m_sprites.at(c.talkAnim.string())->draw();
			}
		} else if (c.visible && !c.noTalkAnim.empty()) {
			m_scene->m_sprites.at(c.noTalkAnim.string())->draw();
		}
	}

	if (m_displayed || fading()) {
		m_scene->m_sprites.at("dialogue_text")->draw();

		if (m_textProgression == 0 && m_delayCounter == 0) {
			if (!m_audioFilesPath.empty()) {
				m_scene->playBGM(m_audioFilesPath.at(m_currentText), m_audioStreamIdx, false);
			} else {
				m_scene->playBGM(fileUtils::path("sound/sfx") / (m_scene->m_game->m_sfx.at(m_sfxSound) + ".wav"), m_audioStreamIdx, true);
			}
		}
		m_delayCounter++;

		if (m_delayCounter == 1 && m_textProgression < m_texts.at(m_currentText).length()) {
			m_delayCounter = 0;
			m_writtenText += m_texts.at(m_currentText).at(m_textProgression);
			m_scene->m_sprites.at("dialogue_text")->setText(m_writtenText);
			m_textProgression++;
			if (!fading()) {
				m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->loop = true;
			}
		} else if (m_textProgression >= m_texts.at(m_currentText).length()) {
			m_scene->m_sprites.at("dialogue_cursor")->m_opacity = m_scene->m_sprites.at("dialogue_text")->m_opacity;
			m_scene->m_sprites.at("dialogue_cursor")->draw();
			m_waiting = true;
			m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->loop = false;
		}
	}
}

void Dialogue::setVisible(bool visible) {
	if (visible == m_displayed) {
		return;
	}

	if (visible) {
		fade(Sprite::FadeInfo{ 250, 0, Sprite::FadingMode::IN });
	} else {
		fade(Sprite::FadeInfo{ 250, 0, Sprite::FadingMode::OUT });

		for (auto& c : m_characters) {
			if (!c.talkAnim.empty()) {
				m_scene->m_sprites.at(c.talkAnim.string())->m_interactive = false;
				m_scene->m_sprites.at(c.noTalkAnim.string())->m_interactive = false;
			}

			m_scene->m_sprites.at(c.nameSprite.string())->m_interactive = false;
			m_scene->m_sprites.at(c.backgroundSprite.string())->m_interactive = false;
		}
		m_scene->m_sprites.at("dialogue_cursor")->m_interactive = false;
	}

	m_displayed = visible;
}

bool Dialogue::waiting() const {
	return m_waiting;
}

bool Dialogue::fading() {
	return m_scene->m_sprites.at("dialogue_text")->m_fading;
}

bool Dialogue::visible() {
	return m_displayed;
}

void Dialogue::skip() {
	m_waiting = true;
	while (m_textProgression < m_texts.at(m_currentText).length()) {
		m_writtenText += m_texts.at(m_currentText).at(m_textProgression);
		m_textProgression++;
	}
	m_scene->m_sprites.at("dialogue_text")->setText(m_writtenText);
	if (m_scene->m_game->m_bgmData.size() > m_audioStreamIdx) {
		m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->loop = false;
	}
}

bool Dialogue::next() {
	if (m_currentText < m_texts.size() - 1) {
		m_currentText++;
		m_textProgression = 0;
		m_delayCounter = 0;
		m_waiting = false;

		if (m_texts.at(m_currentText).substr(0, 3) == U"@c\n") {
			m_writtenText = U"";
			m_textProgression = 0;
			m_texts.at(m_currentText) = m_texts.at(m_currentText).substr(3, m_texts.at(m_currentText).length() - 3);
		}

		if (m_scene->m_game->m_bgmStreams.size() > m_audioStreamIdx) {
			clearAudioStream(m_scene->m_game->m_bgmStreams.at(m_audioStreamIdx), m_scene->m_game->m_bgmData.at(m_audioStreamIdx).get());
		}

		return true;
	}
	return false;
}

void Dialogue::setCharacterVisible(std::string name, bool visible) {
	for (auto& c : m_characters) {
		if (c.name == name) {
			c.visible = visible;
			return;
		}
	}
}

void Dialogue::fade(Sprite::FadeInfo fadeInfo) {
	for (auto& c : m_characters) {
		if (c.talking) {
			m_scene->m_sprites.at(c.backgroundSprite.string())->fade(fadeInfo);
			m_scene->m_sprites.at(c.nameSprite.string())->fade(fadeInfo);

			if (m_scene->m_game->m_bgmData.size() > m_audioStreamIdx && m_scene->m_game->m_bgmData.at(m_audioStreamIdx)->finished && c.visible && !c.noTalkAnim.empty()) {
				m_scene->m_sprites.at(c.noTalkAnim.string())->fade(fadeInfo);
				continue;
			}

			if (c.visible && !c.talkAnim.empty()) {
				m_scene->m_sprites.at(c.talkAnim.string())->fade(fadeInfo);
			}
		} else if (c.visible && !c.noTalkAnim.empty()) {
			m_scene->m_sprites.at(c.noTalkAnim.string())->fade(fadeInfo);
		}
	}

	m_scene->m_sprites.at("dialogue_text")->fade(fadeInfo);
	if (m_texts.size() != 0 && m_textProgression >= m_texts.at(m_currentText).length()) {
		m_scene->m_sprites.at("dialogue_cursor")->fade(fadeInfo);
	}
}

void Dialogue::hideAllCharacters() {
	for (auto& c : m_characters) {
		c.visible = false;
	}
}
