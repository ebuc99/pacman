#include "gamecontroller.h"

GameController *GameController::instance = NULL;

GameController *GameController::getInstance() {
	if (!instance) {
		instance = new GameController();
	}
	return instance;
}

void GameController::cleanUpInstance() {
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

GameController::GameController():
	gameController(NULL)
{
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
		std::cout << "SDL game controller initialization failed: " << SDL_GetError() << std::endl;
	}
}

GameController::~GameController() {
	if (gameController) {
		SDL_GameControllerClose(gameController);
		gameController = NULL;
	}
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

void GameController::searchAndOpen(bool reopen) {
	if (!reopen && gameController)
		return;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			SDL_GameController *controllerToOpen = SDL_GameControllerOpen(i);
			if (controllerToOpen) {
				gameController = controllerToOpen;
				break;
			} else {
				std::cout << "Unable to open game controller " << i << ": " << SDL_GetError() << std::endl;
			}
		}
	}
}

bool GameController::isPresent() {
	if (!gameController)
		searchAndOpen();
	if (!gameController)
		return false;
	return SDL_GameControllerGetAttached(gameController);
}
