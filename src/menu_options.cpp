#include "menu_options.h"

MenuOptions::MenuOptions(Screen *screen, Labyrinth *labyrinth):
	selection(2) {
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
		options_window = TTF_RenderText_Solid(font, "Window 640x480", textgray);
		options_window_sel = TTF_RenderText_Solid(largeFont, "Window 640x480", textwhite);
		options_fullscreen = TTF_RenderText_Solid(font, "Fullscreen", textgray);
		options_fullscreen_sel = TTF_RenderText_Solid(largeFont, "    Fullscreen", textwhite);
		menu = new SDL_Surface*[NUM_MENU_ENTRIES];
		menu_sel = new SDL_Surface*[NUM_MENU_ENTRIES];
		menu[0] = options_sound_on;
		menu_sel[0] = options_sound_on_sel;
		menu[1] = options_window;
		menu_sel[1] = options_window_sel;
		menu[2] = TTF_RenderText_Solid(font, "back", textgray);
		menu_sel[2] = TTF_RenderText_Solid(largeFont, "back", textwhite);
		for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
			menu_entry_rects[i].x = (short int) (320 - (menu_sel[i]->w >> 1));
			menu_entry_rects[i].y = (short int) (290 + i*70 - (menu_sel[i]->h >> 1));
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
	SDL_FreeSurface(options_window);
	SDL_FreeSurface(options_window_sel);
	SDL_FreeSurface(options_fullscreen);
	SDL_FreeSurface(options_fullscreen_sel);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	delete[] menu;
	delete[] menu_sel;
	for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
		SDL_FreeSurface(menu[i]);
		SDL_FreeSurface(menu_sel[i]);
	}
}

void MenuOptions::draw() {
	screen->clear();
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 480;
	screen->draw(optionsTitle, 320 - (optionsTitle->w >> 1), 50);
	drawEntrySelection(selection);
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
					drawEntrySelection(selection);
				}
				else if(event.key.keysym.sym == SDLK_DOWN) {
					selection = ++selection % NUM_MENU_ENTRIES;
					drawEntrySelection(selection);
				}
				else if(event.key.keysym.sym == SDLK_f) {
					screen->toggleFullscreen();
					this->draw();
				}
				else if(event.key.keysym.sym == SDLK_s) {
					labyrinth->getSounds()->toggleEnabled();
					this->draw();
				}
				else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE)) {
					return 2;
				}
				break;
		case SDL_MOUSEMOTION:
			for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
				if (menu_entry_rects[i].x <= event.motion.x && event.motion.x <= menu_entry_rects[i].x+menu_entry_rects[i].w && menu_entry_rects[i].y <= event.motion.y && event.motion.y <= menu_entry_rects[i].y+menu_entry_rects[i].h) {
					drawEntrySelection(selection = i);
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


void MenuOptions::drawEntrySelection(int selection) {
	this->setMenuSelections();
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
		this->draw();
		return 0;
	} else if(selection == 1) {
		screen->toggleFullscreen();
		this->draw();
		return 0;
	} else if(selection == 2)
		return 2;
}

void MenuOptions::setMenuSelections() {
	const int SOUND = 0;
	const int FULLSCREEN = 1;
	if (this->labyrinth->getSounds()->isEnabled()) {
		menu[SOUND] = options_sound_on;
		menu_sel[SOUND] = options_sound_on_sel;
	} else {
		menu[SOUND] = options_sound_off;
		menu_sel[SOUND] = options_sound_off_sel;
	}
	menu_entry_rects[SOUND].w = (short int) options_sound_off_sel->w; 

	if(!screen->isFullscreen()) {
		menu[FULLSCREEN] = options_window;
		menu_sel[FULLSCREEN] = options_window_sel;
	} else {
		menu[FULLSCREEN] = options_fullscreen;
		menu_sel[FULLSCREEN] = options_fullscreen_sel;
	}
	menu_entry_rects[FULLSCREEN].w = (short int) options_window_sel->w;
}