#include "pacman.h"

int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	if(Screen::getInstance()->hasSDLInitErrorOccured())
		return EXIT_FAILURE;

	while(MenuMain::getInstance()->show()) {
		Game::getInstance()->start();
	}

	Game::cleanUpInstance();
	MenuMain::cleanUpInstance();
	Labyrinth::cleanUpInstance();
	Pacman::cleanUpInstance();
	Ghost::cleanUpGhostArray();
	Sounds::cleanUpInstance();
	Screen::cleanUpInstance();

	return EXIT_SUCCESS;
}
