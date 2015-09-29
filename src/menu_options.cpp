#include "menu_options.h"

MenuOptions::MenuOptions():
	Menu("Options") {
	this->addMenuItem("back");
	this->addMenuItem("Window 640x480", "Fullscreen");
	this->addMenuItem("Sound: on", "Sound: off");
}

MenuOptions::~MenuOptions() {
}

int MenuOptions::show() {
	setMenuSelections();
	draw();
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
	return (event == 1 ? 1 : 0);
}

int MenuOptions::handleSelection() {
	if(selection == BACK)
		return 2;
	if(selection == FULLSCREEN)
		screen->toggleFullscreen();
	else if(selection == SOUND)
		Labyrinth::getInstance()->getSounds()->toggleEnabled();
	menuItems.at(selection)->toggleMenuItem();
	this->draw();
	return 0;
}

void MenuOptions::setMenuSelections() {
	if (Labyrinth::getInstance()->getSounds()->isEnabled())
		menuItems.at(SOUND)->setMenuItemAlt(false);
	else
		menuItems.at(SOUND)->setMenuItemAlt(true);

	if(screen->isFullscreen())
		menuItems.at(FULLSCREEN)->setMenuItemAlt(true);
	else 
		menuItems.at(FULLSCREEN)->setMenuItemAlt(false);
}