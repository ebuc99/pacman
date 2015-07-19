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
		static const int NUM_MENU_ENTRIES = 4;
		static const int MIN_FRAME_DURATION = 30;
		SDL_Color textweiss = {255, 255, 255, 0};
		SDL_Color textgray    = {191, 191, 191, 0};
		Screen *screen;
		Labyrinth *labyrinth;
		int eventloop();
		SDL_Surface *optionsTitle;
		SDL_Surface **menu, **menu_sel;
		SDL_Surface *options_sound_on, *options_sound_on_sel, *options_sound_off, *options_sound_off_sel;
		TTF_Font *font, *largeFont, *veryLargeFont;
		SDL_Rect menu_entry_rects[NUM_MENU_ENTRIES];
		void setEntrySelection(int selection);
		int selection;
		int handleSelection();
};

#endif