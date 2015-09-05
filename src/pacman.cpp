#include "pacman.h"


// main function, contains the game loop
int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	if(Screen::getInstance()->hasSDLInitErrorOccured())
		return EXIT_FAILURE;

	// init ghosts
	Ghost *blinky = new Ghost(Constants::BLINKY_INITIAL_X, Constants::BLINKY_INITIAL_Y, Constants::INTELLIGENCE_BLINKY, Figur::LEFT, Constants::INIT_UP_DOWN,       Ghost::BLINKY);
	Ghost *pinky  = new Ghost(Constants::PINKY_INITIAL_X,  Constants::PINKY_INITIAL_Y,  Constants::INTELLIGENCE_PINKY,  Figur::UP,   Constants::INIT_UP_DOWN,       Ghost::PINKY);
	Ghost *inky   = new Ghost(Constants::INKY_INITIAL_X,   Constants::INKY_INITIAL_Y,   Constants::INTELLIGENCE_INKY,   Figur::UP,   Constants::INIT_UP_DOWN_INKY,  Ghost::INKY);
	Ghost *clyde  = new Ghost(Constants::CLYDE_INITIAL_X,  Constants::CLYDE_INITIAL_Y,  Constants::INTELLIGENCE_CLYDE,  Figur::UP,   Constants::INIT_UP_DOWN_CLYDE, Ghost::CLYDE);

	MenuMain *menu = new MenuMain(Screen::getInstance(), Pacman::getInstance(), Ghost::allGhosts, Labyrinth::getInstance());

	while(menu->show()) {
		Game::getInstance()->start();
		menu->draw();
	}

	Game::cleanUpInstance();
	//delete menu;
	Labyrinth::cleanUpInstance();
	Pacman::cleanUpInstance();
	delete blinky;
	delete pinky;
	delete inky;
	delete clyde;
	Sounds::cleanUpInstance();
	Screen::cleanUpInstance();

	return EXIT_SUCCESS;
}
