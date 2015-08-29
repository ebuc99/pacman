#include "menu_about.h"

MenuAbout::MenuAbout(Screen *screen):
		Menu(screen, "About")
{
			idea = TTF_RenderText_Solid(largeFont, "idea: Toro Iwatani - Namco", textgray);
			coding = TTF_RenderText_Solid(largeFont, "coding: Martin Prussak", textgray);
			coding2 = TTF_RenderText_Solid(largeFont, "         Daniel Wuerfel", textgray);
}
MenuAbout::~MenuAbout() {
	SDL_FreeSurface(idea);
}

void MenuAbout::draw() {
	screen->clear();
	this->drawTitle();
	screen->draw(idea, 320 - (idea->w >> 1), 230 - (idea->h >> 1));
	screen->draw(coding, 320 - (coding->w >> 1), 300 - (coding->h >> 1));
	screen->draw(coding2, 320 - (coding2->w >> 1), 340 - (coding2->h >> 1));
	screen->draw(back, 320 - (back->w >> 1), 430 - (back->h >> 1));
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}