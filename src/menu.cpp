#include "menu.h"

Menu::Menu(const char* title):
	selection(0) {
	this->screen = Screen::getInstance();
	if(title)
		menuTitle = TTF_RenderText_Solid(Screen::getVeryLargeFont(), title, Constants::WHITE_COLOR);
}
Menu::~Menu() {
	SDL_FreeSurface(menuTitle);
	for(unsigned int i = 0; i < menuItems.size(); ++i)
		delete menuItems.at(i);
}

void Menu::draw(bool updateAll) {
	screen->clear();
	this->drawTitle();
	this->drawMenuItems();
	if(updateAll)
		screen->AddUpdateRects(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	screen->Refresh();
}

void Menu::drawMenuItems() {
	const int vertical_pad = 35;
	unsigned int i;
	for(i = 0; i < menuItems.size(); ++i) {
		if(selection == i)
			menuItems.at(i)->setSelectMenuItem(true);
		else
			menuItems.at(i)->setSelectMenuItem(false);
		screen->draw(menuItems.at(i)->getCurrentMenuItem(), 320 - (menuItems.at(i)->getCurrentMenuItem()->w >> 1), (430 - (i)*vertical_pad) - (menuItems.at(i)->getCurrentMenuItem()->h >> 1));
	}
	screen->AddUpdateRects(0, (430 - (i)*vertical_pad),
	                       Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT-(430 - (i)*vertical_pad));
}

int Menu::show() {
	draw();
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
	return event;
}

void Menu::drawTitle() {
	if(menuTitle)
		screen->draw_dynamic_content(menuTitle, 320 - (menuTitle->w >> 1), 50);
}

void Menu::addMenuItem(const char* menuItem, const char* menuItemAlt) {
	menuItems.push_back(new MenuItem(menuItem, menuItemAlt));
	menuItems.back()->setXPosition(320 -  (menuItems.back()->getCurrentMenuItem()->w >> 1));
	menuItems.back()->setYPosition(430 - (((int)menuItems.size()-1)*35) - (menuItems.back()->getCurrentMenuItem()->h >> 1));
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
					updateMenuItemNames();
					this->draw();
				}
				else if(event.key.keysym.sym == SDLK_s) {
					Sounds::getInstance()->toggleEnabled();
					updateMenuItemNames();
					this->draw();
				}
				else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
					return 2;
				break;
		case SDL_MOUSEMOTION:
			for(unsigned int i = 0; i < menuItems.size(); ++i) {
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
		// Redraw, when overlapped by foreign window
		if(event.window.event == SDL_WINDOWEVENT_EXPOSED ||
		   event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			Screen::getInstance()->addTotalUpdateRect();
			Screen::getInstance()->Refresh();
		}
	}
	return 0;
}

void Menu::menuItemDown() {
	selection = (selection - 1 + (unsigned int)menuItems.size()) % (unsigned int)menuItems.size();
	draw(false);
}

void Menu::menuItemUp() {
	selection = (selection + 1) % (unsigned int)menuItems.size();
	draw(false);
}

void Menu::menuItemSelect(int selection) {
	this->selection = selection;
	draw(false);
}

MenuItem* Menu::getSelectedMenuItem() {
	return menuItems.at(selection);
}

int Menu::handleSelection() {
	return 0;
}

void Menu::updateMenuItemNames() {}
