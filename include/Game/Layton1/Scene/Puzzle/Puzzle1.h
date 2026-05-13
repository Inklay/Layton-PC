#pragma once

#include "Game/Layton1/Scene/Puzzle/Puzzle.h"

namespace Layton1Scene {

	class Puzzle1 : public Puzzle {
	public:
		Puzzle1(Game* game);
		void load();
		void render();
		void unload();
		void handleClick(const std::string& spriteName, SDL_Event event);
		void handleEvent(SDL_Event event);

	protected:
		bool validate();

	private:
		std::vector<SDL_FPoint> m_points;
		int m_tutoState = 0;
		int m_touchTextState = 0;
		int m_touchTextTimer = 0;
		int m_targetedVillaged = -1;
		SDL_Texture* m_drawTexture;
		SDL_FRect m_drawTextureRect;
		std::vector<SDL_FPoint> m_villages;
		bool m_drawing = false;
	};
};
