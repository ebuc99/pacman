#include "menu_about.h"

MenuAbout::MenuAbout():
	Menu("About") {
		idea = Screen::getTextSurface(Screen::getLargeFont(), "idea: Toro Iwatani - Namco", Constants::GRAY_COLOR);
		coding = Screen::getTextSurface(Screen::getLargeFont(), "coding: Martin Prussak", Constants::GRAY_COLOR);
		coding2 = Screen::getTextSurface(Screen::getLargeFont(), "         Daniel Wuerfel", Constants::GRAY_COLOR);
		this->addMenuItem("back");
}
MenuAbout::~MenuAbout() {
	SDL_FreeSurface(idea);
	SDL_FreeSurface(coding);
	SDL_FreeSurface(coding2);
}


void MenuAbout::draw(bool updateAll) {
	screen->clear();
	this->drawTitle();
	this->drawMenuItems();
	screen->draw(idea, 320 - (idea->w >> 1), 230 - (idea->h >> 1));
	screen->draw(coding, 320 - (coding->w >> 1), 300 - (coding->h >> 1));
	screen->draw(coding2, 320 - (coding2->w >> 1), 340 - (coding2->h >> 1));
	if(updateAll)
		screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int MenuAbout::handleSelection() {
	/*if(selection == BACK)
		return 2;*/
	return selection == BACK ? 2 : 0;
}
