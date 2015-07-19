#include "menu_about.h"

MenuAbout::MenuAbout(Screen *screen) {
	this->screen = screen;
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	largeFont = TTF_OpenFont(filePath, 24);
	if (!largeFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	veryLargeFont = TTF_OpenFont(filePath, 48);
	if (!veryLargeFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	menuTitle = TTF_RenderText_Solid(veryLargeFont, "About", textweiss);
	back = TTF_RenderText_Solid(largeFont, "back", textweiss);
}
MenuAbout::~MenuAbout() {
	SDL_FreeSurface(menuTitle);
	SDL_FreeSurface(back);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
}

void MenuAbout::draw() {
	screen->clear();
	screen->draw(menuTitle, 320 - (menuTitle->w >> 1), 50);
	screen->draw(back, 320 - (back->w >> 1), 430 - (back->h >> 1));
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int MenuAbout::show() {
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
}

int MenuAbout::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
					return 1;
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
