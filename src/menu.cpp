#include "menu.h"

Menu::Menu(Screen *screen) {
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	bigFont = TTF_OpenFont(filePath, 50);
	if(!bigFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	headline = TTF_RenderText_Solid(this->bigFont, "Pacman", this->textweiss);
	screen->draw_dynamic_content(headline, 200, 50);
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

Menu::~Menu() {
	TTF_CloseFont(bigFont);
}

int Menu::show() {
	while(eventloop())
		SDL_Delay(30);
	return 1;
}

int Menu::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_s)
					return 0;
		}
	}
	return 1;
}