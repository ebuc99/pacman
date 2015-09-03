#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "pacman_figur.h"
#include "ghost_figur.h"

const int MIN_FRAME_DURATION  = 30;    // duration of a loop in milliseconds (i.e. minimum time between frames)
const uint16_t START_OFFSET   = 4500;  // waiting time when the level starts, in ms
const uint16_t START_OFFSET_2 = 1500;  // waiting time after respawn, in ms
const int RED                 = 2;     // color red for init text
const int INITIAL_LIVES       = 3;     // number of times the player must die to get the "game over"

class Game {
	private:
		Pacman *pacman;
		SDL_Surface *background, *score;
		TTF_Font *font, *smallFont;
		SDL_Color textweiss = {255, 255, 255, 0};
		int numberGhosts;
		int gameOver;
		int stop_moving;
		int refresh_ghosts;
		uint16_t pause;
		void resetAllFigures();
	public:
		Game(Pacman *pacman);
		~Game();
		void init();
		int getDelayTime(Uint32 *currentTicks);
		int eventloop(bool allowPacmanControl, int *neededTime);
		void start();
		/* stop all figures */
		void stop(uint16_t stop);
		void setGameOver(int gameOver);
		int isGameOver();
};

#endif
