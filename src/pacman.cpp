#include "pacman.h"
#include "config.h"
int main(int argc, char *argv[]) {
	srand((unsigned int)time(0)); // init randomize

	CommandLineOptions::set(argc, argv);
	if (CommandLineOptions::exists("h", "help")) {
		std::cout << "This game is a Pacman clone (version " << VERSION << ")."                             << std::endl
		          << "Usage: pacman [options]"                                  << std::endl
		          << std::endl
		          << "Options:"                                                 << std::endl
		          << "  -h, --help         Display this help message and quit." << std::endl
		          << "  -f, --fullscreen   Start the game in fullscreen mode."  << std::endl
		          << "  -s, --nosound      Start with sound switched off."      << std::endl
		          << std::endl;
		return EXIT_SUCCESS;
	}
	else if (CommandLineOptions::exists("v", "version")) {
		std::cout << "pacman version " << VERSION << std::endl;
		return EXIT_SUCCESS;
	}

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
	CommandLineOptions::cleanUp();

	return EXIT_SUCCESS;
}
