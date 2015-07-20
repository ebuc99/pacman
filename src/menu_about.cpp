#include "menu_about.h"

MenuAbout::MenuAbout(Screen *screen, const char* title):
		Menu(screen, title)
{
			idea = TTF_RenderText_Solid(largeFont, "idea: Toro Iwatani - Namco", textwhite);
}
MenuAbout::~MenuAbout() {
	SDL_FreeSurface(idea);
}

void MenuAbout::draw() {
	screen->clear();
	screen->draw(menuTitle, 320 - (menuTitle->w >> 1), 50);
	screen->draw(idea, 320 - (idea->w >> 1), 230 - (idea->h >> 1));
	screen->draw(back, 320 - (back->w >> 1), 430 - (back->h >> 1));
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}