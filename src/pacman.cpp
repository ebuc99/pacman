#include "pacman.h"


// main function, contains the game loop
int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	if(Screen::getInstance()->hasSDLInitErrorOccured())
		return EXIT_FAILURE;

	MenuMain *menu = new MenuMain(Screen::getInstance(), Pacman::getInstance(), Ghost::getGhostArray(), Labyrinth::getInstance());

	while(menu->show()) {
		Game::getInstance()->start();
		menu->draw();
	}

	Game::cleanUpInstance();
	//delete menu;
	Labyrinth::cleanUpInstance();
	Pacman::cleanUpInstance();
	Ghost::cleanUpGhostArray();
	Sounds::cleanUpInstance();
	Screen::cleanUpInstance();

	return EXIT_SUCCESS;
}
