#pragma once

#include "Game/Scene/Scene.h"
#include <functional>

namespace Layton1Scene {
	class Map : public Scene {
	public:
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);
		void unload();

	protected:
		struct AfterPuzzleDialogue {
			int puzzleNumber;
			int successDialogueId;
			int failDialogueId;
		};

		Map(Game* game, const std::string& bgm, const std::string& internalName);
		void findHintCoin();
		void changeDialogue(int dialogueId);
		bool isFadingToDialogue();
		virtual void renderMap() = 0;
		virtual void renderDialogue() = 0;
		virtual void handleDialogProgression() = 0;
		virtual void loadMap() = 0;
		virtual void loadDialogue() = 0;

		const std::string m_bgm;
		const std::string m_internalName;
		int m_currentDialogueId = -1;
		int m_nextDialogueId = -1;
		int m_dialogueProgression = 0;
		int m_lastPuzzle = -1;
		std::vector<std::vector<bool>> m_hintCoins;
		std::u32string m_questText;
		std::vector<std::pair<std::string, int>> m_dialogues;
		std::vector<std::pair<SDL_FRect, fileUtils::path>> m_hints;
		std::vector<std::pair<SDL_FRect, std::string>> m_puzzles;
		std::function<void()> m_interactionCallBack;
		std::vector<AfterPuzzleDialogue> m_afterPuzzleDialogues;
		bool m_interaction = false;
	};
};
