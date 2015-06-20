#include "pacman.h"




// Constants
const uint16_t INTELLIGENCE_BLINKY = 90;  // intelligence for each ghost
const uint16_t INTELLIGENCE_PINKY = 60;
const uint16_t INTELLIGENCE_INKY = 30;
const uint16_t INTELLIGENCE_CLYDE = 0;
const uint16_t INIT_UP_DOWN = 0;          // initial up/down cycles before the ghost will be allowed to leave the castle
const uint16_t INIT_UP_DOWN_INKY = 5;
const uint16_t INIT_UP_DOWN_CLYDE = 11;
const uint32_t MIN_FRAME_DURATION = 30;        // duration of a loop in milliseconds (i.e. minimum time between frames)
const uint16_t START_OFFSET   = 4500;
const uint16_t START_OFFSET_2 = 1500;
const int INITIAL_LIVES = 3;              // number of times the player must die to get the "game over"
const int RED = 2;                        // color red for init text

// initialize static member variables
int Ghost::was_moving_leader = 1;

Screen *screen;

int stop_moving = 0;
int refresh_ghosts = 0;
uint16_t pause = 0;
float speed_ghosts;
float speed_pacman;

static SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1) {
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filename);
	if(!temp) {
		printf("Unable to load image: %s\n", IMG_GetError());
		exit(-1);
	}
	if(transparent_color != -1)
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparent_color, (uint8_t)transparent_color, (uint8_t)transparent_color));
	surface = SDL_DisplayFormat(temp);
	if(surface == NULL) {
		printf("Unable to convert image to display format: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
        }
    SDL_FreeSurface(temp);
    return surface;	
}

// decide whether something has to be redrawn
static int moving() {
	return (Ghost::was_moving_leader || refresh_ghosts) ? 1 : 0;
}

/* reset all figures */
static void reset_all(Pacman *pacman, Figur **ghost_array) {
	int i;
	for(i = 0; i <= 3; ++i)
		ghost_array[i]->reset();
	pacman->reset();
}
/* stop all figures */
static void stop_all(uint16_t stop, Pacman *pacman, Ghost **ghost_array, Labyrinth *labyrinth) {
	if(stop) {
		ghost_array[0]->set_stop(true);
		ghost_array[1]->set_stop(true);
		ghost_array[2]->set_stop(true);
		ghost_array[3]->set_stop(true);
		pacman->set_stop(true);
		stop_moving = 1;
		labyrinth->getSounds()->music_stop();	
	} else {
		ghost_array[0]->set_stop(false);
		ghost_array[1]->set_stop(false);
		ghost_array[2]->set_stop(false);
		ghost_array[3]->set_stop(false);
		pacman->set_stop(false);
		stop_moving = 0;
		labyrinth->getSounds()->siren_start();
	}
}

// SDL event loop: handle keyboard input events, and others
static int eventloop(Pacman *pacman, Ghost **ghost_array, bool allowPacmanControl, 
                     int *neededTime, Labyrinth *labyrinth) {
	*neededTime = 0;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
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
			    screen->toggleFullscreen();
			    *neededTime = (int) (SDL_GetTicks() - ticksBefore);
			}
			if(event.key.keysym.sym == SDLK_p) {
				if(!pacman->is_dying()) {
					if(pause) {
						pause = 0;
						labyrinth->getSounds()->resume_all();
					} else {
						pause = 1;
						labyrinth->getSounds()->pause_all();
					}
				}
			}
			break;
		case SDL_QUIT:
			return 0;
		}		
	}
	return 1;
}


// main function, contains the game loop
int main(int argc, char *argv[]) {
	SDL_Surface *hintergrund;
	SDL_Surface *score;
	TTF_Font *font, *smallFont;
	SDL_Color textweiss = {255, 255, 255, 0};
	int currentScore = 0, lastScore = 0;
	int loop = 1;
	int start_offset = START_OFFSET;
	Uint32 currentTicks, lastTicks;
	int deltaT;
	int specialEventTime;
	int animation_counter = 0;
	srand((unsigned int)time(0)); // init randomize
	
	// create the window 
	screen = new Screen();
	if(screen->occuredInitSdlError() == EXIT_FAILURE)
		return EXIT_FAILURE;

	//create the labyrinth
	Labyrinth *labyrinth = new Labyrinth(screen);
	

	// create an instance of pacman
	Pacman *pacman = new Pacman(310, 339, screen, labyrinth, INITIAL_LIVES);
	
	// init ghosts
	Ghost *blinky = new Ghost(310, 174, INTELLIGENCE_BLINKY, 
	               			  Figur::LEFT, INIT_UP_DOWN, Ghost::BLINKY,
	                          screen, labyrinth, pacman);
	
	Ghost *pinky = new Ghost(310, 222, INTELLIGENCE_PINKY, 
	              			 Figur::UP, INIT_UP_DOWN, Ghost::PINKY, 
	                         screen, labyrinth, pacman);
	
	Ghost *inky = new Ghost(280, 222, INTELLIGENCE_INKY, 
	             			Figur::UP, INIT_UP_DOWN_INKY, Ghost::INKY, 
	                        screen, labyrinth, pacman);
	
	Ghost *clyde = new Ghost(340, 222, INTELLIGENCE_CLYDE, 
	              			 Figur::UP, INIT_UP_DOWN_CLYDE, Ghost::CLYDE,
	                         screen, labyrinth, pacman);

	// ghost array
	Figur *ghost_array[4] = {blinky, pinky, inky, clyde};
	Ghost *ghost_array_ghost[4] = {blinky, pinky, inky, clyde};


	blinky->setGhostArray(ghost_array_ghost);
	pinky->setGhostArray(ghost_array_ghost);
	inky->setGhostArray(ghost_array_ghost);
	clyde->setGhostArray(ghost_array_ghost);

	// initialize background graphic
	char filePath[256];
	getFilePath(filePath, "gfx/hintergrund2.png");
    hintergrund = LoadSurface(filePath);
	
	// initialize TTF so we are able to write texts to the screen
	if(TTF_Init() == -1) {
		return EXIT_FAILURE;
	}
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	font = TTF_OpenFont(filePath, 20);
	if(!font) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
		return -1;
	}
	smallFont = TTF_OpenFont(filePath, 12);
	if (!smallFont) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
		return -1;
	}
	labyrinth->setFonts(font, smallFont);
	score = TTF_RenderText_Solid(font, "Score", textweiss);
	if(score == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
   
	labyrinth->init_pillen(hintergrund);
	labyrinth->draw_pillen();  // including background
	labyrinth->setInitText("Get Ready!");
	labyrinth->startFruitRandomizer(true);
	labyrinth->setLevel(1);
	pacman->draw();
	blinky->draw();
	pinky->draw();
	inky->draw();
	clyde->draw();

	screen->draw(score, 530, 30);
	pacman->drawLives();
	screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
	screen->Refresh();
	currentTicks = SDL_GetTicks();
	deltaT = MIN_FRAME_DURATION;
	blinky->set_leader(1);	// Blinky will be the reference sprite for redrawing
	// at first, stop all figures 
	stop_all(true, pacman, ghost_array_ghost, labyrinth); 
	//labyrinth->playSoundIntro();
	labyrinth->getSounds()->intro();
	// game loop
	while(loop) {	
		loop = eventloop(pacman, ghost_array_ghost, start_offset<0, &specialEventTime, labyrinth);
		if (specialEventTime > 0)
			currentTicks += specialEventTime;

		animation_counter += deltaT;
		if(animation_counter > 100) {
			// ghost animations
			refresh_ghosts = 1;
			blinky->animation();
			pinky->animation();
			inky->animation();
			clyde->animation();
			
			// Pacman die animation
			if(pacman->is_dying()) {
				if(!pacman->die_animation()) {
					labyrinth->stopHuntingMode();
					labyrinth->hideFruit();
					reset_all(pacman, ghost_array);
					stop_all(true, pacman, ghost_array_ghost, labyrinth);
					pacman->addLives(-1);
					if (pacman->getRemainingLives() <= 0) {
						labyrinth->setInitText("Game over", RED);
						start_offset = -1;  // do not start again
						pacman->setVisibility(0);  // Pacman does not exist anymore
						blinky->setVisibility(0);  // Ghosts should not be shown
						pinky->setVisibility(0);
						inky->setVisibility(0);
						clyde->setVisibility(0);
					} else {
						labyrinth->setInitText("Get Ready!");
						start_offset = START_OFFSET_2;
					}
				    screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
				}
			}
			labyrinth->pill_animation();
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
				labyrinth->hideInitText();
				stop_all(false, pacman, ghost_array_ghost, labyrinth);
			}
	 	}
	 	// hunting mode - ghosts can be eaten after eating a superpill, but only for a defined time
		if(labyrinth->cnt_hunting_mode > 0 && !pause && labyrinth->cnt_sleep <= 0) {
			if (labyrinth->cnt_hunting_mode > 2000 && labyrinth->cnt_hunting_mode-deltaT <= 2000) {
				blinky->blink();
				pinky->blink();
				inky->blink();
				clyde->blink();
			}
			labyrinth->cnt_hunting_mode -= deltaT;
			if (labyrinth->cnt_hunting_mode <= 0) {
				if (!pacman->is_dying()) {
					if (blinky->get_hunter() != Figur::NONE)  // eaten ghosts still have to return to the castle
						blinky->set_hunter(Figur::GHOST);
					if (pinky->get_hunter() != Figur::NONE)
						pinky->set_hunter(Figur::GHOST);
					if (inky->get_hunter() != Figur::NONE)
						inky->set_hunter(Figur::GHOST);
					if (clyde->get_hunter() != Figur::NONE)
						clyde->set_hunter(Figur::GHOST);
				}
				labyrinth->stopHuntingMode();
				labyrinth->getSounds()->siren_start();
			}
		}
		// sleep counter - wait a short time when the player gets a special score (e.g. after eating a ghost or a fruit)
		if (labyrinth->cnt_sleep > 0 && !pause) {
			labyrinth->cnt_sleep -= deltaT;
			if (labyrinth->cnt_sleep <= 0) {
				labyrinth->cnt_sleep = 0;
				labyrinth->hideSmallScore();
				pacman->setVisibility(true);
				blinky->setVisibility(true);
				pinky->setVisibility(true);
				inky->setVisibility(true);
				clyde->setVisibility(true);
			}
		}

		// move all figures
		if (!pause && labyrinth->cnt_sleep <= 0) {
			pacman->move(deltaT);
			blinky->move(deltaT);
			pinky->move(deltaT);
			inky->move(deltaT);
			clyde->move(deltaT);
		}

		// did something special happen during the move?
		pacman->check_eat_pills(ghost_array);
		if(labyrinth->getExisitingPills() <= 0) { 
			// init new level
			reset_all(pacman, ghost_array);
			stop_all(true, pacman, ghost_array_ghost,  labyrinth);
			labyrinth->initNewLevel();
			start_offset = START_OFFSET;
		    labyrinth->draw_pillen();  // including background
		    screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
			continue;
		}
		lastScore = currentScore;
		currentScore = labyrinth->getScore();
		if (lastScore < 10000 && currentScore >= 10000) {
			pacman->addLives(1);
			labyrinth->playSoundExtraMan();
		}
		if(pacman->touch(ghost_array)  && !pacman->is_dying()) {
			stop_all(true, pacman, ghost_array_ghost, labyrinth);
			pacman->set_dying(10);  
		}
		if (!pause && labyrinth->cnt_sleep <= 0) {
			labyrinth->checkFruit(deltaT);
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

		// redrawing
		if (moving()) {
		    // objects within the level
		    labyrinth->draw_pillen();  // including background
			labyrinth->drawFruit();
			// figures
			pacman->animate();
			pacman->draw();
			pacman->addUpdateRect();
			blinky->draw();
			blinky->addUpdateRect();
			pinky->draw();
			pinky->addUpdateRect();
			inky->draw();
			inky->addUpdateRect();
			clyde->draw();
			clyde->addUpdateRect();
			// topmost things within the level
		    labyrinth->draw_blocks();
			labyrinth->drawSmallScore();
			labyrinth->drawInitText();
			// information area: score, lives, the level's fruit
			labyrinth->compute_score();
			screen->draw(score, 530, 30);
			pacman->drawLives();
			labyrinth->drawInfoFruit();
		    // bring it to the screen, really
			screen->Refresh();
		}

		// determine the correct game speed
		lastTicks = currentTicks;
		currentTicks = SDL_GetTicks();
		deltaT = (int) (currentTicks - lastTicks);
		if (deltaT < MIN_FRAME_DURATION) {
			SDL_Delay(MIN_FRAME_DURATION - deltaT);
			deltaT = MIN_FRAME_DURATION;
			currentTicks = lastTicks + MIN_FRAME_DURATION;  // otherwise, the waiting time would cause problems within the next frame
		}
	}
	
	// clean up SDL
    TTF_CloseFont(font);
    TTF_Quit();
	SDL_FreeSurface(hintergrund);

	delete pacman;
	delete blinky;
	delete pinky;
	delete inky;
	delete clyde;
	delete labyrinth;
	delete screen;

	return EXIT_SUCCESS;
}
