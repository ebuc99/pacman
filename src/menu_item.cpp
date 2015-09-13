#include "menu_item.h"

MenuItem::MenuItem(const char* menuItem, const char* menuItemAlt):
	selected(false),
	altMenu(false){
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
	if(menuItemAlt) {
		this->menuItemAlt = TTF_RenderText_Solid(font, menuItemAlt, textgray);
		this->menuItemSelAlt = TTF_RenderText_Solid(largeFont, menuItemAlt, textwhite);
	}
}

MenuItem::~MenuItem() {
	SDL_FreeSurface(menuItem);
	SDL_FreeSurface(menuItemSel);
	SDL_FreeSurface(menuItemAlt);
	SDL_FreeSurface(menuItemSelAlt);
}

SDL_Surface* MenuItem::getCurrentMenuItem() {	 	   
	if(selected)
		return (altMenu ? menuItemSelAlt : menuItemSel);
	else
		return (altMenu ? menuItemAlt : menuItem);
}

void MenuItem::setSelectMenuItem(bool set) {
	selected = set;
}

void MenuItem::toggleMenuItem() {
	if(menuItemAlt)
		altMenu = altMenu ? false : true;
}

void MenuItem::setMenuItemAlt(bool set) {
	altMenu = set;
}

void MenuItem::setXPosition(int x) {
	this->x = x;
}

void MenuItem::setYPosition(int y) {
	this->y = y;
}

int MenuItem::getXPosition() const {
	return this->x;
}

int MenuItem::getYPosition() const {
	return this->y;
}