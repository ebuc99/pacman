#include "menu_item.h"

MenuItem::MenuItem(const char* menuItem, const char* menuItemAlt) {
	textwhite.r = textwhite.g = textwhite.b = 255;
	textgray.r = textgray.g = textgray.b = 192;
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	if(!(font = TTF_OpenFont(filePath, 20)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(largeFont = TTF_OpenFont(filePath, 24)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	this->menuItem = TTF_RenderText_Solid(font, menuItem, textgray);
	this->menuItemSel = TTF_RenderText_Solid(largeFont, menuItem, textwhite);
	if(menuItemAlt != NULL) {
		this->menuItemAlt = TTF_RenderText_Solid(font, menuItemAlt, textgray);
		this->menuItemSelAlt = TTF_RenderText_Solid(largeFont, menuItemAlt, textwhite);
	}
	this->currentMenuItem = this->menuItem;
}

MenuItem::~MenuItem() {
	SDL_FreeSurface(menuItem);
	SDL_FreeSurface(menuItemSel);
	SDL_FreeSurface(menuItemAlt);
	SDL_FreeSurface(menuItemSelAlt);
}

SDL_Surface* MenuItem::getCurrentMenuItem() {
	return this->currentMenuItem;
}

void MenuItem::setSelection(bool selection) {
	if(selection)
		this->currentMenuItem = this->menuItemSel;
	else
		this->currentMenuItem = this->menuItem;
}
void MenuItem::toggleMenuItem() {

}