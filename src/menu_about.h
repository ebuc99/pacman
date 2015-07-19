#ifndef MENU_ABOUT_H
#define MENU_ABOUT_H
#include "screen.h"
#include "platform.h"
#include "menu_options.h"

class MenuAbout {
	public:
		MenuAbout(Screen *screen);
		~MenuAbout();
		void draw();
		int show();
	private:
		static const int MIN_FRAME_DURATION = 30;
		Screen *screen;
		SDL_Color textweiss = {255, 255, 255, 0};
		SDL_Surface *menuTitle, *back;
		TTF_Font *largeFont, *veryLargeFont;
		int eventloop();
};
#endif