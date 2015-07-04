#ifndef MENU_H
#define MENU_H
#include "screen.h"
#include "platform.h"
#include <SDL/SDL_ttf.h>

class Menu {
	public:
		Menu(Screen *screen);
		~Menu();
		int show();	
	private:
		SDL_Color textweiss = {255, 255, 255, 0};
		int eventloop();
		SDL_Surface *headline;
		SDL_Surface *selection[3];
		TTF_Font *bigFont;
		TTF_Font *normalFont;
};
#endif