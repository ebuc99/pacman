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

		void start();

	private:
		Game();
		~Game();
		static Game *instance;

		void init();

		bool gameOver;
		void setGameOver(bool gameOver);

		bool stopMoving;
		void stop(bool stop);  // stop all figures

		bool refreshGhosts;

		bool pause;

		int startOffset;
		Uint32 currentTicks;

		void resetAllFigures();
		bool eventloop();

		int deltaT;
		void updateDelayTime();

		int animationCounter;
		void handleAnimations();

		void handleStartOffset();
		void handleHuntingMode();
		void handleSleep();
		void handleFruit();

		int currentScore;
		bool checkLastPillEaten();
		void checkGhostTouched();
		void checkScoreForExtraLife();
		void checkedMove();
		void checkedRedraw();

		SDL_Surface *background, *score;
		TTF_Font *font, *smallFont;
};

#endif
