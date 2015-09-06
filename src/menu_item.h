#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "platform.h"

class MenuItem {
	public:
		MenuItem(const char* menuItem, const char* menuItemAlt = NULL);
		~MenuItem();
		SDL_Surface* getCurrentMenuItem();
		void setSelection(bool selection = false);
		void toggleMenuItem();
	private:
		SDL_Color textwhite = {255, 255, 255, 0};
		SDL_Color textgray    = {191, 191, 191, 0};
		TTF_Font *font, *largeFont;
		SDL_Surface *currentMenuItem;
		SDL_Surface *menuItem, *menuItemSel; 
		SDL_Surface *menuItemAlt, *menuItemSelAlt;
};
#endif