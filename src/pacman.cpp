#include "pacman.h"


// Constants
const uint16_t INTELLIGENCE_BLINKY = 90;  // intelligence for each ghost
const uint16_t INTELLIGENCE_PINKY = 60;
const uint16_t INTELLIGENCE_INKY = 30;
const uint16_t INTELLIGENCE_CLYDE = 0;
const uint16_t INIT_UP_DOWN = 0;          // initial up/down cycles before the ghost will be allowed to leave the castle
const uint16_t INIT_UP_DOWN_INKY = 5;
const uint16_t INIT_UP_DOWN_CLYDE = 11;


// main function, contains the game loop
int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	if(Screen::getInstance()->occuredInitSdlError() == EXIT_FAILURE)
		return EXIT_FAILURE;

	// create an instance of pacman
	Pacman *pacman = new Pacman(310, 339, INITIAL_LIVES);

	// init ghosts
	Ghost *blinky = new Ghost(310, 174, INTELLIGENCE_BLINKY, Figur::LEFT, INIT_UP_DOWN,       Ghost::BLINKY, pacman);
	Ghost *pinky  = new Ghost(310, 222, INTELLIGENCE_PINKY,  Figur::UP,   INIT_UP_DOWN,       Ghost::PINKY,  pacman);
	Ghost *inky   = new Ghost(280, 222, INTELLIGENCE_INKY,   Figur::UP,   INIT_UP_DOWN_INKY,  Ghost::INKY,   pacman);
	Ghost *clyde  = new Ghost(340, 222, INTELLIGENCE_CLYDE,  Figur::UP,   INIT_UP_DOWN_CLYDE, Ghost::CLYDE,  pacman);

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
