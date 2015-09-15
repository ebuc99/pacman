#include "menu_main.h"

MenuMain::MenuMain(Screen *screen, Pacman *pacman, Ghost *ghosts[], Labyrinth *labyrinth):
	Menu(screen){
		this->pacman = pacman;
		this->labyrinth = labyrinth;
		this->ghosts = ghosts;
		char filePath[256];
		appTitle1 = TTF_RenderText_Solid(hugeFont, "Pa", textwhite);
		appTitle2 = TTF_RenderText_Solid(hugeFont, "man", textwhite);
		getFilePath(filePath, "gfx/title_pacman.png");
		titlePacman = screen->LoadSurface(filePath, 0);
		version = TTF_RenderText_Solid(smallFont, "version 0.7.0", textgray);
		this->addMenuItem("back");
		this->addMenuItem("About");
		this->addMenuItem("Options");
		this->addMenuItem("Start Game");
		menuoptions = new MenuOptions(this->screen, this->labyrinth);
		menuabout = new MenuAbout(this->screen);
		draw();
}

MenuMain::~MenuMain() {
	SDL_FreeSurface(appTitle1);
	SDL_FreeSurface(appTitle2);
	SDL_FreeSurface(titlePacman);
	delete menuoptions;
	delete menuabout;
}

void MenuMain::drawTitle() {
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
	screen->draw(version, (short int)(320 - (version->w >> 1)), 140);
}

int MenuMain::show() {
	int event;
	FunnyAnimation *funnyAnimation = new FunnyAnimation(screen, pacman, ghosts, labyrinth);
	while(!(event = eventloop())) {
		SDL_Delay(MIN_FRAME_DURATION);
		funnyAnimation->animate();
	}
	delete funnyAnimation;
	if(event == 1)
		return 1;
	else
		return 0;
}

int MenuMain::handleSelection() {
	if(selection == STARTGAME)
		return 1;
	else if(selection == OPTIONS) {
		menuoptions->show();
		this->draw();
	}
	else if(selection == ABOUT) {
		menuabout->show();
		this->draw();
	}
	else if(selection == BACK)
		return 2;
	return 0;
}
