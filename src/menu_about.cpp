#include "menu_about.h"

MenuAbout::MenuAbout(Screen *screen):
		Menu(screen, "About")
{
			idea = TTF_RenderText_Solid(largeFont, "idea: Toro Iwatani - Namco", textgray);
			coding = TTF_RenderText_Solid(largeFont, "coding: Martin Prussak", textgray);
			coding2 = TTF_RenderText_Solid(largeFont, "         Daniel Wuerfel", textgray);
			back = TTF_RenderText_Solid(font, "back", textgray);
			back_sel = TTF_RenderText_Solid(largeFont, "back", textwhite);
			test = TTF_RenderText_Solid(font, "test", textgray);
			test_sel = TTF_RenderText_Solid(largeFont, "test", textwhite);
			this->addMenuItem("back");
			this->addMenuItem("test");
}
MenuAbout::~MenuAbout() {
	SDL_FreeSurface(idea);
	SDL_FreeSurface(coding);
	SDL_FreeSurface(coding2);
	SDL_FreeSurface(back);
	SDL_FreeSurface(back_sel);
	SDL_FreeSurface(test);
	SDL_FreeSurface(test_sel);
}


void MenuAbout::draw() {
	screen->clear();
	this->drawTitle();
	this->drawMenuItems();
	screen->draw(idea, 320 - (idea->w >> 1), 230 - (idea->h >> 1));
	screen->draw(coding, 320 - (coding->w >> 1), 300 - (coding->h >> 1));
	screen->draw(coding2, 320 - (coding2->w >> 1), 340 - (coding2->h >> 1));
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int MenuAbout::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
					return 1;
				if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
					return 1;
				if(event.key.keysym.sym == SDLK_UP) {
					menuItemUp();
				}
				else if(event.key.keysym.sym == SDLK_DOWN) {
					menuItemDown();
				}
				break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
					return 1;
			break;
		case SDL_QUIT:
				return 1;
		}
	}
	return 0;
}