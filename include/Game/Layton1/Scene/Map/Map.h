#pragma once

#include "Game/Scene/Scene.h"

namespace Layton1Scene {
	class Map : public Scene {
	public:
		void load();
		void render();
		void handleClick(const std::string& spriteName, SDL_Event event);

	protected:
		Map(Game* game, const std::string& bgm);
		void findHintCoin();
		void changeDialogue(int dialogueId);
		bool isFadingToDialogue();
		virtual void renderMap() = 0;
		virtual void renderDialogue() = 0;
		virtual void handleDialogProgression() = 0;
		virtual void loadMap() = 0;
		virtual void loadDialogue() = 0;

		const std::string m_bgm;
		int m_currentDialogueId = -1;
		int m_nextDialogueId = -1;
		int m_dialogueProgression = 0;
		std::vector<std::vector<bool>> m_hintCoins;
	};
};
