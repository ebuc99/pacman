#include "menu_main.h"

Menu::Menu(Screen *screen, Pacman *pacman):
	selection(0){
	this->screen = screen;
	this->pacman = pacman;
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
	hugeFont = TTF_OpenFont(filePath, 96);
	if (!hugeFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	appTitle1 = TTF_RenderText_Solid(hugeFont, "Pa", textweiss);
	appTitle2 = TTF_RenderText_Solid(hugeFont, "man", textweiss);
	getFilePath(filePath, "gfx/title_pacman.png");
    titlePacman = screen->LoadSurface(filePath, 0);
	menu = new SDL_Surface*[NUM_MENU_ENTRIES];
	menu_sel = new SDL_Surface*[NUM_MENU_ENTRIES];
	menu[0] = TTF_RenderText_Solid(font, "Start Game", textgray);
	menu_sel[0] = TTF_RenderText_Solid(largeFont, "Start Game", textweiss);
	menu[1] = TTF_RenderText_Solid(font, "Options", textgray);
	menu_sel[1] = TTF_RenderText_Solid(largeFont, "Options", textweiss);
	menu[2] = TTF_RenderText_Solid(font, "About", textgray);
	menu_sel[2] = TTF_RenderText_Solid(largeFont, "About", textweiss);
	menu[3] = TTF_RenderText_Solid(font, "Quit", textgray);
	menu_sel[3] = TTF_RenderText_Solid(largeFont, "Quit", textweiss);
	for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
		menu_entry_rects[i].x = (short int) (320 - (menu_sel[i]->w >> 1));
		menu_entry_rects[i].y = (short int) (325 + i*35 - (menu_sel[i]->h >> 1));
		menu_entry_rects[i].w = (short int) menu_sel[i]->w;
		menu_entry_rects[i].h = (short int) menu_sel[i]->h;
	}
	draw(screen);
}

Menu::~Menu() {
	SDL_FreeSurface(appTitle1);
	SDL_FreeSurface(appTitle2);
	SDL_FreeSurface(titlePacman);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	TTF_CloseFont(hugeFont);
	delete[] menu;
	delete[] menu_sel;
}
void Menu::draw(Screen *screen) {
	screen->clear();
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 480;
	rect.x = (short int) (320 - ((appTitle1->w + titlePacman->w + appTitle2->w) >> 1));
	screen->draw(appTitle1, rect.x, 30);
	rect.x = (short int) (rect.x + appTitle1->w + titlePacman->w);
	screen->draw(appTitle2, rect.x, 30);
	rect.x = (short int) (rect.x - titlePacman->w);
	screen->draw(titlePacman, rect.x, 40);
	setEntrySelection(selection);
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int Menu::show() {
	int event;
	SDL_Rect animRect;
	animRect.x = 0;
	animRect.y = 200;
	animRect.w = 640;
	animRect.h = 23;
	while(!(event = eventloop())) {
		SDL_Delay(MIN_FRAME_DURATION);
		lastAnimTime = animationTime;
		animationTime += MIN_FRAME_DURATION;
		if (idxAnimation == 0) {
			// Animation 1: Pacman comes from the right and leaves to the left.
			if (animationTime >= ANIMATION_WAIT) {
				if (lastAnimTime < ANIMATION_WAIT) {
					animWaitUntil = 0;
					pacman->reset();
					pacman->set_position(640, 200);
				}
				pacman->move_left(MIN_FRAME_DURATION, -23);
				if (pacman->x <= -23) {
					idxAnimation  = (idxAnimation + 1) % NUM_ANIMATIONS;
					animationTime = 0;
					animationPart = 0;
					screen->AddUpdateRects(animRect.x, animRect.y, animRect.w, animRect.h);
				}
				screen->fillRect(&animRect, 0, 0, 0);
				pacman->animate();
				pacman->draw();
				pacman->addUpdateRect();
				screen->Refresh();
			}					
		}
	}
	if(event == 1)
		return 1;
	else //quit
		return 0;
}

int Menu::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN) {
					if(selection == 0)
						return 1;
					else if (selection == 3)
						return 2;
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
		case SDL_QUIT:
				return 2;
		}
	}
	return 0;
}

void Menu::setEntrySelection(int selection) {
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