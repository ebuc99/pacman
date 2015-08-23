#ifndef MENU_ABOUT_H
#define MENU_ABOUT_H
#include "menu.h"

class MenuAbout : public Menu {
	public:
		MenuAbout(Screen *screen, const char* title);
		~MenuAbout();
		void draw();
	private:
		SDL_Surface *idea;
		SDL_Surface *coding;
		SDL_Surface *coding2;
};
#endif