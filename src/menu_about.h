#ifndef MENU_ABOUT_H
#define MENU_ABOUT_H
#include "menu.h"

class MenuAbout : public Menu {
	public:
		MenuAbout(Screen *screen);
		~MenuAbout();
		void draw();
	private:
		int eventloop();
		SDL_Surface *back, *back_sel, *test, *test_sel;
		SDL_Surface *idea, *coding, *coding2;
};
#endif