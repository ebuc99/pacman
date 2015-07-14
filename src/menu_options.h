#ifndef MENU_OPTIONS
#define MENU_OPTIONS
#include "screen.h"
#include "platform.h"
#include <SDL/SDL_ttf.h>

class MenuOptions {
	public:
		MenuOptions(Screen *screen);
		~MenuOptions();
		void draw();
		int show();
	private:
		static const int NUM_OPTIONS_ENTRIES = 4;
		static const int MIN_FRAME_DURATION = 30;
		SDL_Color textweiss = {255, 255, 255, 0};
		SDL_Color textgray    = {191, 191, 191, 0};
		SDL_Color textyellow  = {255, 255,   0, 0};
		SDL_Color textred     = {255,   0,   0, 0};
		SDL_Color textmagenta = {255,   0, 255, 0};
		SDL_Color textcyan    = {  0, 255, 255, 0};
		SDL_Color textorange  = {255, 127,   0, 0};
		Screen *screen;
		int eventloop();
		SDL_Surface *optionsTitle;
		SDL_Surface **menu, **menu_sel;
		SDL_Surface *options_sound_on, *options_sound_on_sel, *options_sound_off, *options_sound_off_sel;
		TTF_Font *font, *largeFont, *veryLargeFont;
		SDL_Rect options_entry_rects[NUM_OPTIONS_ENTRIES];
		void setEntrySelection(int selection);
		int selection;
};

#endif