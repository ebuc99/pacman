#include "menu_main.h"
#include "config.h"

MenuMain* MenuMain::instance = NULL;

MenuMain* MenuMain::getInstance() {
	if(!instance)
		instance = new MenuMain();
	return instance;
}

void MenuMain::cleanUpInstance() {
	if(instance) {
		delete instance;
		instance = NULL;
	}
}
MenuMain::MenuMain() {
		SDL_Surface *icon = Screen::loadImage("gfx/pacman_desktop.png", 0);
		SDL_SetWindowIcon(Screen::getInstance()->getWindow(), icon);
		SDL_FreeSurface(icon);
		this->menuTitle = NULL;
		appTitle1 = TTF_RenderText_Solid(Screen::getHugeFont(), "Pa", Constants::WHITE_COLOR);
		appTitle2 = TTF_RenderText_Solid(Screen::getHugeFont(), "man", Constants::WHITE_COLOR);
		titlePacman = Screen::loadImage("gfx/title_pacman.png", 0);
		std::string str_version = "version ";
		str_version.append(VERSION);
		version = TTF_RenderText_Solid(Screen::getSmallFont(), str_version.c_str(), Constants::GRAY_COLOR);
		this->addMenuItem("Quit");
		this->addMenuItem("About");
		this->addMenuItem("Highscore List");
		this->addMenuItem("Options");
		this->addMenuItem("Start Game");
		this->selection = STARTGAME;
		menuoptions = new MenuOptions();
		menuabout = new MenuAbout();
		draw();
}

MenuMain::~MenuMain() {
	SDL_FreeSurface(appTitle1);
	SDL_FreeSurface(appTitle2);
	SDL_FreeSurface(titlePacman);
	SDL_FreeSurface(version);
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
	draw();
	int event;
	while(!(event = eventloop())) {
		SDL_Delay(MIN_FRAME_DURATION);
		FunnyAnimation::getInstance()->animate();
	}
	FunnyAnimation::cleanUpInstance();
	return (event == 1 ? 1 : 0);
}

int MenuMain::handleSelection() {
	if(selection == STARTGAME)
		return 1;
	else if(selection == OPTIONS) {
		menuoptions->show();
		this->draw();
	}
	else if(selection == HIGHSCORE) {
		HighscoreList::getInstance()->load();
		HighscoreList::getInstance()->show(false, false);
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
