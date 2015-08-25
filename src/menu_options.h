#ifndef MENU_OPTIONS
#define MENU_OPTIONS
#include "screen.h"
#include "platform.h"
#include "labyrinth.h"
#include <SDL/SDL_ttf.h>

class MenuOptions {
	public:
		MenuOptions(Screen *screen, Labyrinth *labyrinth);
		~MenuOptions();
		void draw();
		int show();
	private:
		static const int NUM_MENU_ENTRIES = 3;
		static const int MIN_FRAME_DURATION = 30;
		SDL_Color textwhite;
		SDL_Color textgray;
		Screen *screen;
		Labyrinth *labyrinth;
		int eventloop();
		SDL_Surface *optionsTitle;
		SDL_Surface **menu, **menu_sel;
		SDL_Surface *options_sound_on, *options_sound_on_sel, *options_sound_off, *options_sound_off_sel;
		SDL_Surface *options_window, *options_window_sel, *options_fullscreen, *options_fullscreen_sel;
		TTF_Font *font, *largeFont, *veryLargeFont;
		SDL_Rect menu_entry_rects[NUM_MENU_ENTRIES];
		void drawEntrySelection(int selection);
		void setMenuSelections();
		int selection;
		int handleSelection();
};

#endif