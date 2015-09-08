#include "game.h"
#include <iostream>

Game *Game::instance = NULL;

Game *Game::getInstance() {
	if (!instance) {
		instance = new Game();
	}
	return instance;
}

void Game::cleanUpInstance() {
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

Game::Game():
	gameOver(false),
	stopMoving(false),
	refreshGhosts(false),
	pause(false)
{
	// initialize background graphic
	char filePath[256];
	getFilePath(filePath, "gfx/hintergrund2.png");
    background = Screen::getInstance()->LoadSurface(filePath);
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	font = TTF_OpenFont(filePath, 20);
	if(!font) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	smallFont = TTF_OpenFont(filePath, 12);
	if (!smallFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	}
	Labyrinth::getInstance()->setFonts(font, smallFont);
	score = TTF_RenderText_Solid(font, "Score", Constants::WHITE_COLOR);
	if(score == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
	}
}

Game::~Game() {
	SDL_FreeSurface(background);
	TTF_CloseFont(font);
}

void Game::resetAllFigures() {
	for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
		Ghost::getGhostArray()[i]->reset();
	Pacman::getInstance()->reset();
}

void Game::init() {
	Labyrinth::getInstance()->startFruitRandomizer(true);
	Labyrinth::getInstance()->setLevel(1);
	Labyrinth::getInstance()->setInitText("Get Ready!");
	Labyrinth::getInstance()->init_pillen(background, true);
	Labyrinth::getInstance()->draw_pillen();  // including background
	resetAllFigures();
	Ghost::getGhostArray()[0]->set_leader(1);  // Blinky will be the reference sprite for redrawing
	Pacman::getInstance()->setRemainingLives(Constants::INITIAL_LIVES);
	Labyrinth::getInstance()->resetScore();
	currentScore = Labyrinth::getInstance()->getScore();
	Screen::getInstance()->draw(score, Constants::SCORE_X, Constants::SCORE_Y);
	Pacman::getInstance()->drawLives();
	Screen::getInstance()->addTotalUpdateRect();
	Screen::getInstance()->Refresh();
	gameOver         = false;
	refreshGhosts    = false;
	stopMoving       = false;
	pause            = false;
	stopMoving       = true;
	animationCounter = 0;
	deltaT           = Constants::MIN_FRAME_DURATION;
	startOffset      = Constants::START_OFFSET;
	currentTicks     = SDL_GetTicks();
}

void Game::updateDelayTime() {
	// determine the correct game speed
	Uint32 lastTicks = currentTicks;
	currentTicks = SDL_GetTicks();
	deltaT = (int) (currentTicks - lastTicks);
	if (deltaT < Constants::MIN_FRAME_DURATION) {
		SDL_Delay(Constants::MIN_FRAME_DURATION - deltaT);
		deltaT = Constants::MIN_FRAME_DURATION;
		currentTicks = lastTicks + Constants::MIN_FRAME_DURATION;  // otherwise, the waiting time would cause problems within the next frame
	}
}

bool Game::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(gameOver)
				return false;
			if(!Pacman::getInstance()->is_dying() && !pause) {
				if(event.key.keysym.sym == SDLK_LEFT)
					Pacman::getInstance()->direction_pre = Figur::LEFT;
				if(event.key.keysym.sym == SDLK_UP)
					Pacman::getInstance()->direction_pre = Figur::UP;
				if(event.key.keysym.sym == SDLK_RIGHT)
					Pacman::getInstance()->direction_pre = Figur::RIGHT;
				if(event.key.keysym.sym == SDLK_DOWN)
					Pacman::getInstance()->direction_pre = Figur::DOWN;
			}
			if(event.key.keysym.sym == SDLK_f) {
				Uint32 ticksBefore = SDL_GetTicks();
			    Screen::getInstance()->toggleFullscreen();
			    currentTicks += (SDL_GetTicks() - ticksBefore);
			} else if(event.key.keysym.sym == SDLK_s) {
				Sounds::getInstance()->toggleEnabled();
			} else if(event.key.keysym.sym == SDLK_p) {
				if(!Pacman::getInstance()->is_dying()) {
					pause = !pause;
					if(pause) {
						Sounds::getInstance()->pause_all();
					} else {
						Sounds::getInstance()->resume_all();
					}
				}
			} else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE)) {
				return false;
			}
			break;
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}

void Game::stop(bool stop) {
	stopMoving = stop;
	if(stop) {
		Sounds::getInstance()->music_stop();
		Sounds::getInstance()->channelStop();
	} else {
		Sounds::getInstance()->siren_start();
	}
}

void Game::setGameOver(bool gameOver) {
	this->gameOver = gameOver;
	if(gameOver) {
		stop(true);
		Labyrinth::getInstance()->setInitText("Game over", Constants::RED);
	}
}

void Game::start() {
	init();
	Sounds::getInstance()->intro();
	// game loop
	while (eventloop()) {
		handleAnimations();

		// handle time based counters
		handleStartOffset();
		handleHuntingMode();
		handleSleep();
		handleFruit();

		// move all figures, if they are allowed to move - and check, what happened
		checkedMove();
		Pacman::getInstance()->check_eat_pills();
		checkScoreForExtraLife();
		checkedRedraw();
		if (checkLastPillEaten())
			continue;
		checkGhostTouched();

		updateDelayTime();
	}
	stop(true);
}

void Game::handleAnimations() {
	animationCounter += deltaT;
	if(animationCounter > 100) {
		// ghost animations
		refreshGhosts = true;
		for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
			Ghost::getGhostArray()[i]->animation();

		// Pacman die animation
		if(Pacman::getInstance()->is_dying()) {
			if(!Pacman::getInstance()->die_animation()) {
				Labyrinth::getInstance()->stopHuntingMode();
				Labyrinth::getInstance()->hideFruit();
				resetAllFigures();
				stop(true);
				Pacman::getInstance()->addLives(-1);
				if (Pacman::getInstance()->getRemainingLives() <= 0) {
					setGameOver(true);
					startOffset = -1;  // do not start again
					Pacman::getInstance()->setVisibility(false);  // Pacman does not exist anymore
					for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
						Ghost::getGhostArray()[i]->setVisibility(false);
				} else {
					Labyrinth::getInstance()->setInitText("Get Ready!");
					startOffset = Constants::START_OFFSET_2;
				}
				Screen::getInstance()->AddUpdateRects(0, 0, background->w, background->h);
			}
		}
		Labyrinth::getInstance()->pill_animation();
		animationCounter -= 100;
	} else {
		refreshGhosts = false;
	}
}

void Game::handleStartOffset() {
	// The "start offset" is a short wait time before the game begins, so the player may prepare to play.
	if(startOffset > 0) {
		startOffset -= deltaT;
		if (startOffset <= 0) {
			startOffset = -1;
			Labyrinth::getInstance()->hideInitText();
			stop(false);
		}
	}
}

void Game::handleHuntingMode() {
	// During the "hunting mode", ghosts can be eaten after eating a superpill, but only for a defined time.
	if(Labyrinth::getInstance()->cnt_hunting_mode > 0 && !pause && Labyrinth::getInstance()->cnt_sleep <= 0) {
		if (Labyrinth::getInstance()->cnt_hunting_mode > 2000 && Labyrinth::getInstance()->cnt_hunting_mode-deltaT <= 2000) {
			for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
				Ghost::getGhostArray()[i]->blink();
		}
		Labyrinth::getInstance()->cnt_hunting_mode -= deltaT;
		if (Labyrinth::getInstance()->cnt_hunting_mode <= 0) {
			if (!Pacman::getInstance()->is_dying()) {
				for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i) {
					if (Ghost::getGhostArray()[i]->get_hunter() != Figur::NONE)  // eaten ghosts still have to return to the castle
						Ghost::getGhostArray()[i]->set_hunter(Figur::GHOST);
				}
			}
			Labyrinth::getInstance()->stopHuntingMode();
			Sounds::getInstance()->siren_start();
		}
	}
}

void Game::handleSleep() {
	// The "sleep counter" becomes active when the player got a special score (e.g. after eating a ghost or a fruit).
	if (Labyrinth::getInstance()->cnt_sleep > 0 && !pause) {
		Labyrinth::getInstance()->cnt_sleep -= deltaT;
		if (Labyrinth::getInstance()->cnt_sleep <= 0) {
			Labyrinth::getInstance()->cnt_sleep = 0;
			Labyrinth::getInstance()->hideSmallScore();
			Pacman::getInstance()->setVisibility(true);
			for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
				Ghost::getGhostArray()[i]->setVisibility(true);
			Sounds::getInstance()->eat_ghost_start();
		}
	}
}

void Game::handleFruit() {
	// The fruit will only be displayed for a defined period. After that, it will disappear if it still has not been eaten.
	if (!pause && Labyrinth::getInstance()->cnt_sleep <= 0) {
		Labyrinth::getInstance()->checkFruit(deltaT);
	}
}

bool Game::checkLastPillEaten() {
	if(Labyrinth::getInstance()->getExisitingPills() <= 0) {
		// init new level
		resetAllFigures();
		stop(true);
		Labyrinth::getInstance()->initNewLevel();
		startOffset = Constants::START_OFFSET;
		Labyrinth::getInstance()->draw_pillen();  // including background
		Screen::getInstance()->AddUpdateRects(0, 0, background->w, background->h);
		return true;
	}
	return false;
}

void Game::checkScoreForExtraLife() {
	int lastScore = currentScore;
	currentScore = Labyrinth::getInstance()->getScore();
	if (lastScore < 10000 && currentScore >= 10000) {
		Pacman::getInstance()->addLives(1);
		Labyrinth::getInstance()->playSoundExtraMan();
	}
}

void Game::checkedMove() {
	if (!pause && Labyrinth::getInstance()->cnt_sleep <= 0 && !stopMoving) {
		Pacman::getInstance()->move(deltaT);
		for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i)
			Ghost::getGhostArray()[i]->move(deltaT);
	}
}

void Game::checkedRedraw() {
	// redraw, if something has to be redrawn
	if (Ghost::was_moving_leader || refreshGhosts) {
		// objects within the level
		Labyrinth::getInstance()->draw_pillen();  // including background
		Labyrinth::getInstance()->drawFruit();
		// figures
		Pacman::getInstance()->animate();
		Pacman::getInstance()->draw();
		Pacman::getInstance()->addUpdateRect();
		for(int i = 0; i < Constants::TOTAL_NUM_GHOSTS; ++i) {
			Ghost::getGhostArray()[i]->draw();
			Ghost::getGhostArray()[i]->addUpdateRect();
		}
		// topmost things within the level
		Labyrinth::getInstance()->draw_blocks();
		Labyrinth::getInstance()->drawSmallScore();
		Labyrinth::getInstance()->drawInitText();
		// information area: score, lives, the level's fruit
		Labyrinth::getInstance()->compute_score();
		Screen::getInstance()->draw(score, Constants::SCORE_X, Constants::SCORE_Y);
		Pacman::getInstance()->drawLives();
		Labyrinth::getInstance()->drawInfoFruit();
		// bring it to the screen, really
		Screen::getInstance()->Refresh();
	}
}

void Game::checkGhostTouched() {
	if(Pacman::getInstance()->ghostTouched() && !Pacman::getInstance()->is_dying()) {
		stop(true);
		Pacman::getInstance()->set_dying(10);
	}
}
