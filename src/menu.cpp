#include "menu.h"

Menu::Menu(Screen *screen) {
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	bigFont = TTF_OpenFont(filePath, 50);
	if(!bigFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	normalFont = TTF_OpenFont(filePath, 20);
	if(!normalFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	headline = TTF_RenderText_Solid(this->bigFont, "Pacman", this->textweiss);
	selection[0] = TTF_RenderText_Solid(this->normalFont, "Start Game", this->textweiss);
	selection[1] = TTF_RenderText_Solid(this->normalFont, "Controls", this->textweiss);
	selection[2] = TTF_RenderText_Solid(this->normalFont, "About", this->textweiss);
	screen->draw_dynamic_content(headline, 200, 50);
	screen->draw_dynamic_content(selection[0], 240, 300);
	screen->draw_dynamic_content(selection[1], 260, 350);
	screen->draw_dynamic_content(selection[2], 280, 400);
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
				if(event.key.keysym.sym == SDLK_RETURN)
					return 0;
		case SDL_QUIT:
				return 0;
		}
	}
	return 1;
}