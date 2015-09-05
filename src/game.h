#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "constants.h"
#include "pacman_figur.h"
#include "ghost_figur.h"

class Game {
	public:
		static Game *getInstance();
		static void cleanUpInstance();
		void init();
		int getDelayTime(Uint32 *currentTicks);
		int eventloop(bool allowPacmanControl, int *neededTime);
		void start();
		/* stop all figures */
		void stop(uint16_t stop);
		void setGameOver(int gameOver);
		int isGameOver();
	private:
		Game();
		~Game();
		static Game *instance;
		SDL_Surface *background, *score;
		TTF_Font *font, *smallFont;
		SDL_Color textweiss = {255, 255, 255, 0};
		int gameOver;
		int stop_moving;
		int refresh_ghosts;
		uint16_t pause;
		void resetAllFigures();
};

#endif
