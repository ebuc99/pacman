#include "pacman.h"


// main function, contains the game loop
int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	if(Screen::getInstance()->occuredInitSdlError() == EXIT_FAILURE)
		return EXIT_FAILURE;

	// create an instance of pacman
	Pacman *pacman = new Pacman(310, 339, Constants::INITIAL_LIVES);

	// init ghosts
	Ghost *blinky = new Ghost(310, 174, Constants::INTELLIGENCE_BLINKY, Figur::LEFT, Constants::INIT_UP_DOWN,       Ghost::BLINKY, pacman);
	Ghost *pinky  = new Ghost(310, 222, Constants::INTELLIGENCE_PINKY,  Figur::UP,   Constants::INIT_UP_DOWN,       Ghost::PINKY,  pacman);
	Ghost *inky   = new Ghost(280, 222, Constants::INTELLIGENCE_INKY,   Figur::UP,   Constants::INIT_UP_DOWN_INKY,  Ghost::INKY,   pacman);
	Ghost *clyde  = new Ghost(340, 222, Constants::INTELLIGENCE_CLYDE,  Figur::UP,   Constants::INIT_UP_DOWN_CLYDE, Ghost::CLYDE,  pacman);

	Game *game = new Game(pacman);
	MenuMain *menu = new MenuMain(Screen::getInstance(), pacman, Ghost::allGhosts, Labyrinth::getInstance());

	while(menu->show()) {
		game->start();
		menu->draw();
	}

	delete pacman;
	delete blinky;
	delete pinky;
	delete inky;
	delete clyde;
	//delete menu;
	delete game;
	Labyrinth::cleanUpInstance();
	Screen::cleanUpInstance();
	Sounds::cleanUpInstance();

	return EXIT_SUCCESS;
}
