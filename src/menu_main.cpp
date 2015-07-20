#include "menu_main.h"

MenuMain::MenuMain(Screen *screen, Pacman *pacman, Ghost *ghosts[], Labyrinth *labyrinth):
	selection(0){
		this->screen = screen;
		this->pacman = pacman;
		this->labyrinth = labyrinth;
		this->ghosts = ghosts;
		char filePath[256];
		getFilePath(filePath, "fonts/Cheapmot.TTF");
		font = TTF_OpenFont(filePath, 20);
		if(!font) {
			printf("Unable to open TTF font: %s\n", TTF_GetError());
		}
		smallFont = TTF_OpenFont(filePath, 12);
		if (!smallFont) {
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
		animScore200 = TTF_RenderText_Solid(smallFont, "200", textweiss);
		animScore400 = TTF_RenderText_Solid(smallFont, "400", textweiss);
		animScore800 = TTF_RenderText_Solid(smallFont, "800", textweiss);
		animScore1600 = TTF_RenderText_Solid(smallFont, "1600", textweiss);
		for (int i = 0; i < NUM_MENU_ENTRIES; i++) {
			menu_entry_rects[i].x = (short int) (320 - (menu_sel[i]->w >> 1));
			menu_entry_rects[i].y = (short int) (325 + i*35 - (menu_sel[i]->h >> 1));
			menu_entry_rects[i].w = (short int) menu_sel[i]->w;
			menu_entry_rects[i].h = (short int) menu_sel[i]->h;
		}
		menuoptions = new MenuOptions(this->screen, this->labyrinth);
		menuabout = new MenuAbout(this->screen, "About");
		draw();
}

MenuMain::~MenuMain() {
	SDL_FreeSurface(appTitle1);
	SDL_FreeSurface(appTitle2);
	SDL_FreeSurface(titlePacman);
	SDL_FreeSurface(animScore200);
	SDL_FreeSurface(animScore400);
	SDL_FreeSurface(animScore800);
	SDL_FreeSurface(animScore1600);
	TTF_CloseFont(font);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	TTF_CloseFont(hugeFont);
	delete[] menu;
	delete[] menu_sel;
	delete menuoptions;
	delete menuabout;
}
void MenuMain::draw() {
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

int MenuMain::show() {
	int event;
	SDL_Rect animRect;
	animRect.x = 0;
	animRect.y = 200;
	animRect.w = 640;
	animRect.h = 23;
	int animation_counter = 0;
	int timeScore200 = 0, timeScore400 = 0, timeScore800 = 0, timeScore1600 = 0;
	int xScore200 = 0, xScore400 = 0, xScore800 = 0, xScore1600 = 0;
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
							
		} else if (idxAnimation == 1) {
				// Animation 2: Pacman comes from the left, followed by blinky, pinky, inky and clyde.
				//              Pacman eats the superpill on the right, turns around and eats each ghost.
				if (animationTime >= ANIMATION_WAIT) {
					if (lastAnimTime < ANIMATION_WAIT) {
						animationPart = 1;
						animWaitUntil = 0;
						// initialize figures
						pacman->reset();
						pacman->set_position(-23, 200);
						ghosts[0]->reset();
						ghosts[0]->set_position(-60, 200);
						ghosts[1]->reset();
						ghosts[1]->set_position(-90, 200);
						ghosts[2]->reset();
						ghosts[2]->set_position(-120, 200);
						ghosts[3]->reset();
						ghosts[3]->set_position(-150, 200);
						animation_counter = 0;
					}
					if (animationPart == 1) {
						pacman->move_right(MIN_FRAME_DURATION, 540);
						if (pacman->x >= 540) {
							animationPart = 2;
							ghosts[0]->set_hunter(Figur::PACMAN);
							ghosts[1]->set_hunter(Figur::PACMAN);
							ghosts[2]->set_hunter(Figur::PACMAN);
							ghosts[3]->set_hunter(Figur::PACMAN);
						}
						ghosts[0]->move_right(MIN_FRAME_DURATION, 640);
						ghosts[1]->move_right(MIN_FRAME_DURATION, 640);
						ghosts[2]->move_right(MIN_FRAME_DURATION, 640);
						ghosts[3]->move_right(MIN_FRAME_DURATION, 640);
						animation_counter += MIN_FRAME_DURATION;
					} else if (animationPart == 2) {
						if (animationTime >= animWaitUntil) {
							pacman->move_left(MIN_FRAME_DURATION, -23);
							pacman->setVisibility(true);
							if (pacman->x <= -23) {
								idxAnimation  = (idxAnimation + 1) % NUM_ANIMATIONS;
								animationTime = 0;
								animationPart = 0;
								timeScore200  = 0;
								timeScore400  = 0;
								timeScore800  = 0;
								timeScore1600 = 0;
								screen->AddUpdateRects(animRect.x, animRect.y, animRect.w, animRect.h);
							}
							ghosts[0]->move_left(MIN_FRAME_DURATION, -23);
							ghosts[1]->move_left(MIN_FRAME_DURATION, -23);
							ghosts[2]->move_left(MIN_FRAME_DURATION, -23);
							ghosts[3]->move_left(MIN_FRAME_DURATION, -23);
						}
						if (pacman->x <= ghosts[0]->x + 11 && ghosts[0]->isVisible()) {
							ghosts[0]->setVisibility(false);
							xScore200 = ghosts[0]->x;
							timeScore200 = 400;
							pacman->setVisibility(false);
							animWaitUntil = animationTime + 400;
						}
						if (pacman->x <= ghosts[1]->x + 11 && ghosts[1]->isVisible()) {
							ghosts[1]->setVisibility(false);
							xScore400 = ghosts[1]->x;
							timeScore400 = 400;
							pacman->setVisibility(false);
							animWaitUntil = animationTime + 400;
						}
						if (pacman->x <= ghosts[2]->x + 11 && ghosts[2]->isVisible()) {
							ghosts[2]->setVisibility(false);
							xScore800 = ghosts[2]->x;
							timeScore800 = 400;
							pacman->setVisibility(false);
							animWaitUntil = animationTime + 400;
						}
						if (pacman->x <= ghosts[3]->x + 11 && ghosts[3]->isVisible()) {
							ghosts[3]->setVisibility(false);
							xScore1600 = ghosts[3]->x;
							timeScore1600 = 400;
							pacman->setVisibility(false);
							animWaitUntil = animationTime + 400;
						}
						animation_counter += MIN_FRAME_DURATION;
					}
					screen->fillRect(&animRect, 0, 0, 0);
					if (animation_counter > 100) {
						animation_counter -= 100;
						ghosts[0]->animation();
						ghosts[1]->animation();
						ghosts[2]->animation();
						ghosts[3]->animation();
						labyrinth->pill_animation();
					}
					if (animationPart == 1) {
						// superpill not yet eaten
						screen->draw(labyrinth->get_superpill_sf(), 546, 206);
						screen->AddUpdateRects(546, 206, labyrinth->get_superpill_sf()->w, labyrinth->get_superpill_sf()->h);
					}
					ghosts[0]->draw();
					ghosts[0]->addUpdateRect();
					ghosts[1]->draw();
					ghosts[1]->addUpdateRect();
					ghosts[2]->draw();
					ghosts[2]->addUpdateRect();
					ghosts[3]->draw();
					ghosts[3]->addUpdateRect();
					pacman->animate();
					pacman->draw();
					pacman->addUpdateRect();
					if (timeScore200 > 0) {
						timeScore200 -= MIN_FRAME_DURATION;
						if (timeScore200 <= 0) {
							timeScore200 = 0;
						} else {
							screen->draw(animScore200, xScore200, 205);
						}
						screen->AddUpdateRects(xScore200, 205, animScore200->w, animScore200->h);
					}
					if (timeScore400 > 0) {
						timeScore400 -= MIN_FRAME_DURATION;
						if (timeScore400 <= 0) {
							timeScore400 = 0;
						} else {
							screen->draw(animScore400, xScore400, 205);
						}
						screen->AddUpdateRects(xScore400, 205, animScore400->w, animScore400->h);
					}
					if (timeScore800 > 0) {
						timeScore800 -= MIN_FRAME_DURATION;
						if (timeScore800 <= 0) {
							timeScore800 = 0;
						} else {
							screen->draw(animScore800, xScore800, 205);
						}
						screen->AddUpdateRects(xScore800, 205, animScore800->w, animScore800->h);
					}
					if (timeScore1600 > 0) {
						timeScore1600 -= MIN_FRAME_DURATION;
						if (timeScore1600 <= 0) {
							timeScore1600 = 0;
						} else {
							screen->draw(animScore1600, xScore1600, 205);
						}
						screen->AddUpdateRects(xScore1600, 205, animScore1600->w, animScore1600->h);
					}
					screen->Refresh();
				}
			}//Ende Animation 1
	}
	if(event == 1)
		return 1;
	else //quit
		return 0;
}

int MenuMain::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
					return handleSelection();
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

void MenuMain::setEntrySelection(int selection) {
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

int MenuMain::handleSelection() {
	if(selection == 0)
			return 1;
		else if(selection == 1) {
			menuoptions->draw();
			menuoptions->show();
			this->draw();
		}
		else if(selection == 2) {
			menuabout->draw();
			menuabout->show();
			this->draw();
		}
		else if(selection == 3)
			return 2;
	return 0;
}