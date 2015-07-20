#include "menu_options.h"

MenuOptions::MenuOptions(Screen *screen, Labyrinth *labyrinth):
	selection(0) {
		this->screen = screen;
		this->labyrinth = labyrinth;
		textwhite.r = textwhite.g = textwhite.b = 255;
		textgray.r = textgray.g = textgray.b = 192;
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
		optionsTitle = TTF_RenderText_Solid(veryLargeFont, "Options", textwhite);
		options_sound_on = TTF_RenderText_Solid(font, "Sound: on", textgray);
		options_sound_on_sel = TTF_RenderText_Solid(largeFont, "Sound: on", textwhite);
		options_sound_off = TTF_RenderText_Solid(font, "Sound: off", textgray);
		options_sound_off_sel = TTF_RenderText_Solid(largeFont, "Sound: off", textwhite);
		menu = new SDL_Surface*[NUM_MENU_ENTRIES];
		menu_sel = new SDL_Surface*[NUM_MENU_ENTRIES];
		menu[0] = options_sound_on;
		menu_sel[0] = options_sound_on_sel;
		menu[1] = TTF_RenderText_Solid(font, "Key configuration", textgray);
		menu_sel[1] = TTF_RenderText_Solid(largeFont, "Key configuration", textwhite);
		menu[2] = TTF_RenderText_Solid(font, "Screen resolution: 640x480", textgray);
		menu_sel[2] = TTF_RenderText_Solid(largeFont, "Screen resolution: 640x480", textwhite);
		menu[3] = TTF_RenderText_Solid(font, "back", textgray);
		menu_sel[3] = TTF_RenderText_Solid(largeFont, "back", textwhite);
		for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
			menu_entry_rects[i].x = (short int) (320 - (menu_sel[i]->w >> 1));
			menu_entry_rects[i].y = (short int) (220 + i*70 - (menu_sel[i]->h >> 1));
			menu_entry_rects[i].w = (short int) menu_sel[i]->w;
			menu_entry_rects[i].h = (short int) menu_sel[i]->h;
		}
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
					return handleSelection();
				}
				else if(event.key.keysym.sym == SDLK_UP) {
					selection = (--selection + NUM_MENU_ENTRIES) % NUM_MENU_ENTRIES;
					setEntrySelection(selection);
				}
				else if(event.key.keysym.sym == SDLK_DOWN) {
					selection = ++selection % NUM_MENU_ENTRIES;
					setEntrySelection(selection);
				}
				break;
		case SDL_MOUSEMOTION:
			for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
				if (menu_entry_rects[i].x <= event.motion.x && event.motion.x <= menu_entry_rects[i].x+menu_entry_rects[i].w && menu_entry_rects[i].y <= event.motion.y && event.motion.y <= menu_entry_rects[i].y+menu_entry_rects[i].h) {
					setEntrySelection(selection = i);
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
					if (menu_entry_rects[i].x <= event.motion.x && event.motion.x <= menu_entry_rects[i].x+menu_entry_rects[i].w && menu_entry_rects[i].y <= event.motion.y && event.motion.y <= menu_entry_rects[i].y+menu_entry_rects[i].h) {
						return handleSelection();
					}
				}
			}
			break;
		case SDL_QUIT:
				return 2;
		}
	}
	return 0;
}


void MenuOptions::setEntrySelection(int selection) {
	for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
		screen->fillRect(&menu_entry_rects[i], 0, 0, 0);
		if(selection == i)
			screen->draw(menu_sel[i], menu_entry_rects[i].x, menu_entry_rects[i].y);
		else
			screen->draw(menu[i], 320-(menu[i]->w >> 1), menu_entry_rects[i].y + ((menu_sel[i]->h - menu[i]->h) >> 1));
		screen->AddUpdateRects(menu_entry_rects[i].x, menu_entry_rects[i].y, menu_entry_rects[i].w, menu_entry_rects[i].h);
	}
	screen->Refresh();
}

int MenuOptions::handleSelection() {
	if(selection == 0) {
		labyrinth->getSounds()->toggleEnabled();
		if (this->labyrinth->getSounds()->isEnabled()) {
			menu[selection] = options_sound_on;
			menu_sel[selection] = options_sound_on_sel;
		} else {
			menu[selection] = options_sound_off;
			menu_sel[selection] = options_sound_off_sel;
		}
		menu_entry_rects[selection].w = (short int) options_sound_off_sel->w; 
		setEntrySelection(selection);
		return 0;
	}else if(selection == 3)
		return 2;
}