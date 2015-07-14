#include "menu_options.h"

MenuOptions::MenuOptions(Screen *screen):
	selection(0) {
		this->screen = screen;
		char filePath[256];
		getFilePath(filePath, "fonts/Cheapmot.TTF");
		font = TTF_OpenFont(filePath, 20);
		if(!font) {
			printf("Unable to open TTF font: %s\n", TTF_GetError());
		}
		largeFont = TTF_OpenFont(filePath, 24);
		if (!largeFont) {
			printf("Unable to open TTF font: %s\n", TTF_GetError());
		}
		veryLargeFont = TTF_OpenFont(filePath, 48);
		if (!veryLargeFont) {
			printf("Unable to open TTF font: %s\n", TTF_GetError());
		}
		optionsTitle = TTF_RenderText_Solid(veryLargeFont, "Options", textweiss);
		options_sound_on = TTF_RenderText_Solid(font, "Sound: on", textgray);
		options_sound_on_sel = TTF_RenderText_Solid(largeFont, "Sound: on", textweiss);
		options_sound_off = TTF_RenderText_Solid(font, "Sound: off", textgray);
		options_sound_off_sel = TTF_RenderText_Solid(largeFont, "Sound: off", textweiss);
		menu = new SDL_Surface*[NUM_OPTIONS_ENTRIES];
		menu_sel = new SDL_Surface*[NUM_OPTIONS_ENTRIES];
		menu[0] = options_sound_on;
		menu_sel[0] = options_sound_on_sel;
		menu[1] = TTF_RenderText_Solid(font, "Key configuration", textgray);
		menu_sel[1] = TTF_RenderText_Solid(largeFont, "Key configuration", textweiss);
		menu[2] = TTF_RenderText_Solid(font, "Screen resolution: 640x480", textgray);
		menu_sel[2] = TTF_RenderText_Solid(largeFont, "Screen resolution: 640x480", textweiss);
		menu[3] = TTF_RenderText_Solid(font, "back", textgray);
		menu_sel[3] = TTF_RenderText_Solid(largeFont, "back", textweiss);
		for (int i = 0; i < NUM_OPTIONS_ENTRIES; i++) {
			options_entry_rects[i].x = (short int) (320 - (menu_sel[i]->w >> 1));
			options_entry_rects[i].y = (short int) (220 + i*70 - (menu_sel[i]->h >> 1));
			options_entry_rects[i].w = (short int) menu_sel[i]->w;
			options_entry_rects[i].h = (short int) menu_sel[i]->h;
		}
		//draw();
}

MenuOptions::~MenuOptions() {
	SDL_FreeSurface(optionsTitle);
	SDL_FreeSurface(options_sound_on);
	SDL_FreeSurface(options_sound_on_sel);
	SDL_FreeSurface(options_sound_off);
	SDL_FreeSurface(options_sound_off_sel);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	delete[] menu;
	delete[] menu_sel;
}

void MenuOptions::draw() {
	screen->clear();
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 480;
	screen->draw(optionsTitle, 320 - (optionsTitle->w >> 1), 50);
	/*rect.x = (short int) (320 - ((optionsTitle->w + titlePacman->w + appTitle2->w) >> 1));
	screen->draw(appTitle1, rect.x, 30);
	rect.x = (short int) (rect.x + appTitle1->w + titlePacman->w);
	screen->draw(appTitle2, rect.x, 30);
	rect.x = (short int) (rect.x - titlePacman->w);
	screen->draw(titlePacman, rect.x, 40);*/
	setEntrySelection(selection);
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int MenuOptions::show() {
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
}

int MenuOptions::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN) {
					if (selection == 3)
						return 2;
				}
				else if(event.key.keysym.sym == SDLK_UP) {
					selection = (--selection + NUM_OPTIONS_ENTRIES) % NUM_OPTIONS_ENTRIES;
					setEntrySelection(selection);
				}
				else if(event.key.keysym.sym == SDLK_DOWN) {
					selection = ++selection % NUM_OPTIONS_ENTRIES;
					setEntrySelection(selection);
				}
				break;
		case SDL_QUIT:
				return 2;
		}
	}
	return 0;
}


void MenuOptions::setEntrySelection(int selection) {
	for (int i = 0; i < NUM_OPTIONS_ENTRIES; i++) {
		screen->fillRect(&options_entry_rects[i], 0, 0, 0);
		if(selection == i)
			screen->draw(menu_sel[i], options_entry_rects[i].x, options_entry_rects[i].y);
		else
			screen->draw(menu[i], 320-(menu[i]->w >> 1), options_entry_rects[i].y + ((menu_sel[i]->h - menu[i]->h) >> 1));
		screen->AddUpdateRects(options_entry_rects[i].x, options_entry_rects[i].y, options_entry_rects[i].w, options_entry_rects[i].h);
	}
	screen->Refresh();
}
