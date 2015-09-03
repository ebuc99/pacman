#ifndef MENU_MAIN_H
#define MENU_MAIN_H
#include "pacman_figur.h"
#include "ghost_figur.h"
#include "menu_options.h"
#include "menu_about.h"

class MenuMain : public Menu {
	public:
		MenuMain(Screen *screen, Pacman *pacman, Ghost *ghosts[], Labyrinth *labyrinth);
		~MenuMain();
		void draw();
		int show();
	private:
		static const int NUM_MENU_ENTRIES = 4;
		static const int MIN_FRAME_DURATION = 30;
		static const int ANIMATION_WAIT = 3000;
		static const int NUM_ANIMATIONS = 5;
		static const int NUM_FRUITS = 8;
		Screen *screen;
		Menu *menuoptions;
		Menu *menuabout;
		Pacman *pacman;
		Ghost **ghosts;
		int eventloop();
		void setEntrySelection(int selection);
		SDL_Rect menu_entry_rects[NUM_MENU_ENTRIES];
		SDL_Surface *headline, *appTitle1, *appTitle2, *titlePacman, *version;
		SDL_Surface **menu, **menu_sel;
		SDL_Surface *animScore200, *animScore400, *animScore800, *animScore1600;
		SDL_Surface *pacmanName, *blinkyName, *pinkyName, *inkyName, *clydeName;
		SDL_Surface **animFruits, **animScores;
		int selection;
		Labyrinth *labyrinth;
		int handleSelection();
};
#endif
