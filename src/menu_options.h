#ifndef MENU_OPTIONS
#define MENU_OPTIONS
#include "labyrinth.h"
#include "menu.h"

class MenuOptions : public Menu {
	public:
		MenuOptions(Screen *screen, Labyrinth *labyrinth);
		~MenuOptions();
		int show();
	private:
		enum Entries {BACK, FULLSCREEN, SOUND};
		Labyrinth *labyrinth;
		void setMenuSelections();
		int handleSelection();	
};

#endif