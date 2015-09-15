#ifndef MENU_MAIN_H
#define MENU_MAIN_H
#include "pacman_figur.h"
#include "ghost_figur.h"
#include "menu_options.h"
#include "menu_about.h"
#include "funny_animation.h"

class MenuMain : public Menu {
	public:
		MenuMain(Screen *screen, Pacman *pacman, Ghost *ghosts[], Labyrinth *labyrinth);
		~MenuMain();
		int show();
	private:
		enum Entries {BACK, ABOUT, OPTIONS, STARTGAME};
		Menu *menuoptions;
		Menu *menuabout;
		Pacman *pacman;
		Ghost **ghosts;
		SDL_Surface *appTitle1, *appTitle2, *titlePacman, *version;
		Labyrinth *labyrinth;
		int handleSelection();
		void drawTitle();
};
#endif
