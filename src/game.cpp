#include "game.h"

Game::Game(Pacman *pacman):
	numberGhosts(4),
	gameOver(0),
	stop_moving(0),
	refresh_ghosts(0),
	pause(0) {
	this->pacman = pacman;
	this->background = background;
	// initialize background graphic
	char filePath[256];
	getFilePath(filePath, "gfx/hintergrund2.png");
    background = Screen::getInstance()->LoadSurface(filePath);
	if(TTF_Init() == -1) {
		printf("Unable to init TTF: %s\n", TTF_GetError());
	}
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
	score = TTF_RenderText_Solid(font, "Score", textweiss);
	if(score == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
	}
}

Game::~Game() {
	SDL_FreeSurface(background);
	TTF_CloseFont(font);
    TTF_Quit();
}

void Game::resetAllFigures() {
	for(int i = 0; i < 4; ++i)
		Ghost::allGhosts[i]->reset();
	pacman->reset();
}

void Game::init() {
	Labyrinth::getInstance()->init_pillen(background, true);
	Labyrinth::getInstance()->draw_pillen();  // including background
	Labyrinth::getInstance()->setInitText("Get Ready!");
	Labyrinth::getInstance()->startFruitRandomizer(true);
	Labyrinth::getInstance()->setLevel(1);
	resetAllFigures();
	pacman->setVisibility(1);
	pacman->draw();
	for (int i = 0; i < numberGhosts; ++i) {
		Ghost::allGhosts[i]->setVisibility(1);
		Ghost::allGhosts[i]->draw();
	}
	pacman->setRemainingLives(INITIAL_LIVES);
	Labyrinth::getInstance()->resetScore();
	setGameOver(0);
	Screen::getInstance()->draw(score, 530, 30);
	pacman->drawLives();
	Screen::getInstance()->AddUpdateRects(0, 0, background->w, background->h);
	Screen::getInstance()->Refresh();
}

int Game::getDelayTime(Uint32 *currentTicks) {
	// determine the correct game speed
	Uint32 lastTicks = *currentTicks;
	*currentTicks = SDL_GetTicks();
	int deltaT = (int) (*currentTicks - lastTicks);
	if (deltaT < MIN_FRAME_DURATION) {
		SDL_Delay(MIN_FRAME_DURATION - deltaT);
		deltaT = MIN_FRAME_DURATION;
		*currentTicks = lastTicks + MIN_FRAME_DURATION;  // otherwise, the waiting time would cause problems within the next frame
	}
	return deltaT;

}

int Game::eventloop(bool allowPacmanControl, int *neededTime) {
	*neededTime = 0;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(isGameOver())
				return 0;
			if(allowPacmanControl && !pacman->is_dying() && !pause) {
				if(event.key.keysym.sym == SDLK_LEFT)
					pacman->direction_pre = Figur::LEFT;
				if(event.key.keysym.sym == SDLK_UP)
					pacman->direction_pre = Figur::UP;
				if(event.key.keysym.sym == SDLK_RIGHT)
					pacman->direction_pre = Figur::RIGHT;
				if(event.key.keysym.sym == SDLK_DOWN)
					pacman->direction_pre = Figur::DOWN;
			}
			if(event.key.keysym.sym == SDLK_f) {
				Uint32 ticksBefore = SDL_GetTicks();
			    Screen::getInstance()->toggleFullscreen();
			    *neededTime = (int) (SDL_GetTicks() - ticksBefore);
			/*} else if(event.key.keysym.sym == SDLK_s) {
				Sounds::getInstance()->toggleEnabled();*/
			} else if(event.key.keysym.sym == SDLK_p) {
				if(!pacman->is_dying()) {
					if(pause) {
						pause = 0;
						Sounds::getInstance()->resume_all();
					} else {
						pause = 1;
						Sounds::getInstance()->pause_all();
					}
				}
			} else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE)) {
				this->stop(1);
				return 0;
			}
			break;
		case SDL_QUIT:
			this->stop(1);
			return 0;
		}
	}
	return 1;
}

void Game::stop(uint16_t stop) {
	if(stop) {
		pacman->set_stop(true);
		for (int i = 0; i < 4; ++i)
			Ghost::allGhosts[i]->set_stop(true);
		stop_moving = 1;
		Sounds::getInstance()->music_stop();
		Sounds::getInstance()->channelStop();
	} else {
		pacman->set_stop(false);
		for (int i = 0; i < 4; ++i)
			Ghost::allGhosts[i]->set_stop(false);
		stop_moving = 0;
		Sounds::getInstance()->siren_start();
	}
}

void Game::setGameOver(int gameOver) {
	this->gameOver = gameOver;
	if(gameOver)
		Labyrinth::getInstance()->setInitText("Game over", RED);
}

int Game::isGameOver() {
	return this->gameOver;
}

void Game::start() {
	init();
	int start_offset = START_OFFSET;
	int specialEventTime;
	int animation_counter = 0;
	int currentScore = 0, lastScore = 0;
	Uint32 currentTicks = SDL_GetTicks();
	int deltaT = MIN_FRAME_DURATION;
	Ghost::allGhosts[0]->set_leader(1);  // Blinky will be the reference sprite for redrawing
	stop(true);  // at first, stop all figures
	Sounds::getInstance()->intro();
	// game loop
	int loop = 1;
	while (loop) {
		int i;
		loop = eventloop(start_offset<0, &specialEventTime);
		if (specialEventTime > 0)
			currentTicks += specialEventTime;

		animation_counter += deltaT;
		if(animation_counter > 100) {
			// ghost animations
			refresh_ghosts = 1;
			for(i = 0; i < numberGhosts; ++i)
				Ghost::allGhosts[i]->animation();

			// Pacman die animation
			if(pacman->is_dying()) {
				if(!pacman->die_animation()) {
					Labyrinth::getInstance()->stopHuntingMode();
					Labyrinth::getInstance()->hideFruit();
					resetAllFigures();
					stop(true);
					pacman->addLives(-1);
					if (pacman->getRemainingLives() <= 0) {
						setGameOver(1);
						start_offset = -1;  // do not start again
						pacman->setVisibility(0);  // Pacman does not exist anymore
						for(i = 0; i < numberGhosts; ++i)
							Ghost::allGhosts[i]->setVisibility(0);
					} else {
						Labyrinth::getInstance()->setInitText("Get Ready!");
						start_offset = START_OFFSET_2;
					}
				    Screen::getInstance()->AddUpdateRects(0, 0, background->w, background->h);
				}
			}
			Labyrinth::getInstance()->pill_animation();
			animation_counter -= 100;
		}
		else
			refresh_ghosts = 0;

		// handle time based counters
		// start offset - wait a short time before the game begins, so the player may prepare to play
		if(start_offset > 0) {
			start_offset -= deltaT;
			if (start_offset <= 0) {
				start_offset = -1;
				Labyrinth::getInstance()->hideInitText();
				stop(false);
			}
	 	}
	 	// hunting mode - ghosts can be eaten after eating a superpill, but only for a defined time
		if(Labyrinth::getInstance()->cnt_hunting_mode > 0 && !pause && Labyrinth::getInstance()->cnt_sleep <= 0) {
			if (Labyrinth::getInstance()->cnt_hunting_mode > 2000 && Labyrinth::getInstance()->cnt_hunting_mode-deltaT <= 2000) {
				for(i = 0; i < numberGhosts; ++i)
					Ghost::allGhosts[i]->blink();
			}
			Labyrinth::getInstance()->cnt_hunting_mode -= deltaT;
			if (Labyrinth::getInstance()->cnt_hunting_mode <= 0) {
				if (!pacman->is_dying()) {
					for(i = 0; i < numberGhosts; ++i) {
						if (Ghost::allGhosts[i]->get_hunter() != Figur::NONE)  // eaten ghosts still have to return to the castle
							Ghost::allGhosts[i]->set_hunter(Figur::GHOST);
					}
				}
				Labyrinth::getInstance()->stopHuntingMode();
				Sounds::getInstance()->siren_start();
			}
		}
		// sleep counter - wait a short time when the player gets a special score (e.g. after eating a ghost or a fruit)
		if (Labyrinth::getInstance()->cnt_sleep > 0 && !pause) {
			Labyrinth::getInstance()->cnt_sleep -= deltaT;
			if (Labyrinth::getInstance()->cnt_sleep <= 0) {
				Labyrinth::getInstance()->cnt_sleep = 0;
				Labyrinth::getInstance()->hideSmallScore();
				pacman->setVisibility(true);
				for(i = 0; i < numberGhosts; ++i)
					Ghost::allGhosts[i]->setVisibility(true);
				Sounds::getInstance()->eat_ghost_start();
			}
		}

		// move all figures
		if (!pause && Labyrinth::getInstance()->cnt_sleep <= 0) {
			pacman->move(deltaT);
			for(i = 0; i < numberGhosts; ++i)
				Ghost::allGhosts[i]->move(deltaT);
		}

		// did something special happen during the move?
		pacman->check_eat_pills();
		if(Labyrinth::getInstance()->getExisitingPills() <= 0) {
			// init new level
			resetAllFigures();
			stop(true);
			Labyrinth::getInstance()->initNewLevel();
			start_offset = START_OFFSET;
		    Labyrinth::getInstance()->draw_pillen();  // including background
		    Screen::getInstance()->AddUpdateRects(0, 0, background->w, background->h);
			continue;
		}
		lastScore = currentScore;
		currentScore = Labyrinth::getInstance()->getScore();
		if (lastScore < 10000 && currentScore >= 10000) {
			pacman->addLives(1);
			Labyrinth::getInstance()->playSoundExtraMan();
		}
		if(pacman->ghostTouched()  && !pacman->is_dying()) {
			stop(true);
			pacman->set_dying(10);
		}
		if (!pause && Labyrinth::getInstance()->cnt_sleep <= 0) {
			Labyrinth::getInstance()->checkFruit(deltaT);
		}

	  	// if pacman stops, please set it to "normal"
		if(pacman->is_pacman_stopped() && !pacman->is_dying()) {
			switch(pacman->get_direction()) {
				case Figur::LEFT:
					pacman->left_pic(0);
					break;
				case Figur::UP:
					pacman->up_pic(0);
					break;
				case Figur::RIGHT:
					pacman->right_pic(0);
					break;
				case Figur::DOWN:
					pacman->down_pic(0);
			}
			pacman->parking();
		}

		// redraw, if something has to be redrawn
		if (Ghost::was_moving_leader || refresh_ghosts) {
		    // objects within the level
		    Labyrinth::getInstance()->draw_pillen();  // including background
			Labyrinth::getInstance()->drawFruit();
			// figures
			pacman->animate();
			pacman->draw();
			pacman->addUpdateRect();
			for(i = 0; i < numberGhosts; ++i) {
				Ghost::allGhosts[i]->draw();
				Ghost::allGhosts[i]->addUpdateRect();
			}
			// topmost things within the level
		    Labyrinth::getInstance()->draw_blocks();
			Labyrinth::getInstance()->drawSmallScore();
			Labyrinth::getInstance()->drawInitText();
			// information area: score, lives, the level's fruit
			Labyrinth::getInstance()->compute_score();
			Screen::getInstance()->draw(score, 530, 30);
			pacman->drawLives();
			Labyrinth::getInstance()->drawInfoFruit();
		    // bring it to the screen, really
			Screen::getInstance()->Refresh();
		}
		deltaT = getDelayTime(&currentTicks);
	}
	pacman->setVisibility(0);
	for(int i = 0; i < numberGhosts; ++i) {
		Ghost::allGhosts[i]->setVisibility(0);
	}
}
