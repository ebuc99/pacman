#ifndef MENU_H
#define MENU_H
#include "screen.h"
#include "platform.h"
#include <SDL/SDL_ttf.h>

class Menu {
	public:
		Menu(Screen *screen, const char* title = NULL);
		~Menu();
		virtual void draw();
		virtual int show();
		void drawTitle();
	private:
		static const int MIN_FRAME_DURATION = 30;
		int eventloop();
	protected:
		Screen *screen;
		SDL_Surface *menuTitle, *back;
		SDL_Color textwhite;
		SDL_Color textgray;
		TTF_Font *font, *smallFont, *largeFont, *veryLargeFont, *hugeFont;
};
#endif