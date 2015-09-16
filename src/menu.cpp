#include "menu.h"

Menu::Menu(Screen *screen, const char* title):
	selection(0) {
	this->screen = screen;
	textwhite.r = textwhite.g = textwhite.b = 255;
	textgray.r = textgray.g = textgray.b = 192;
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	if(!(font = TTF_OpenFont(filePath, 20)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(smallFont = TTF_OpenFont(filePath, 12)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(largeFont = TTF_OpenFont(filePath, 24)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(veryLargeFont = TTF_OpenFont(filePath, 48)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(hugeFont = TTF_OpenFont(filePath, 96)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if(title)
		menuTitle = TTF_RenderText_Solid(veryLargeFont, title, textwhite);
}
Menu::~Menu() {
	SDL_FreeSurface(menuTitle);
	for(int i = 0; i < menuItems.size(); ++i)
		delete menuItems.at(i);
	TTF_CloseFont(font);
	TTF_CloseFont(smallFont);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	TTF_CloseFont(hugeFont);
}

void Menu::draw(bool updateAll) {
	screen->clear();
	this->drawTitle();
	this->drawMenuItems();
	if(updateAll)
		screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

void Menu::drawMenuItems() {
	for(int i = 0; i < menuItems.size(); ++i) {
		if(selection == i)
			menuItems.at(i)->setSelectMenuItem(true);
		else
			menuItems.at(i)->setSelectMenuItem(false);
		screen->draw(menuItems.at(i)->getCurrentMenuItem(), 320 - (menuItems.at(i)->getCurrentMenuItem()->w >> 1), (430 - (i)*35) - (menuItems.at(i)->getCurrentMenuItem()->h >> 1));
	}
	screen->AddUpdateRects(0, 300, 640, 180); // update lower area of the menu, no good coding
}

int Menu::show() {
	draw();
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
}

void Menu::drawTitle() {
	if(menuTitle)
		screen->draw_dynamic_content(menuTitle, 320 - (menuTitle->w >> 1), 50);
}

void Menu::addMenuItem(const char* menuItem, const char* menuItemAlt) {
	menuItems.push_back(new MenuItem(menuItem, menuItemAlt));
	menuItems.back()->setXPosition(320 -  (menuItems.back()->getCurrentMenuItem()->w >> 1));
	menuItems.back()->setYPosition(430 - ((menuItems.size()-1)*35) - (menuItems.back()->getCurrentMenuItem()->h >> 1));
}

int Menu::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
					return handleSelection();
				else if(event.key.keysym.sym == SDLK_UP)
					menuItemUp();
				else if(event.key.keysym.sym == SDLK_DOWN)
					menuItemDown();
				else if(event.key.keysym.sym == SDLK_f) {
					screen->toggleFullscreen();
					this->draw();
				}
				/*else if(event.key.keysym.sym == SDLK_s) {
					labyrinth->getSounds()->toggleEnabled();
					this->draw();
				}*/
				else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
					return 2;
				break;
		case SDL_MOUSEMOTION:
			for(int i = 0; i < menuItems.size(); ++i) {
				if(menuItems.at(i)->getXPosition() <= event.motion.x && event.motion.x <= menuItems.at(i)->getXPosition()+menuItems.at(i)->getCurrentMenuItem()->w && menuItems.at(i)->getYPosition() <= event.motion.y && event.motion.y <= menuItems.at(i)->getYPosition()+menuItems.at(i)->getCurrentMenuItem()->h) {
					menuItemSelect(i);
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
					if(menuItems.at(selection)->getXPosition() <= event.motion.x && 
					   event.motion.x <= menuItems.at(selection)->getXPosition()+menuItems.at(selection)->getCurrentMenuItem()->w && 
					   menuItems.at(selection)->getYPosition() <= event.motion.y && 
					   event.motion.y <= menuItems.at(selection)->getYPosition()+menuItems.at(selection)->getCurrentMenuItem()->h)
						return handleSelection();
			}
			break;
		case SDL_QUIT:
				return 2;
		}
	}
	return 0;
}

void Menu::menuItemDown() {
	selection = (--selection + menuItems.size()) % menuItems.size();
	draw(false);
}

void Menu::menuItemUp() {
	selection = ++selection % menuItems.size();
	draw(false);
}

void Menu::menuItemSelect(int selection) {
	this->selection = selection;
	draw(false);
}

MenuItem* Menu::getSelectedMenuItem() {
	return menuItems.at(selection);	
}

int Menu::handleSelection() {}