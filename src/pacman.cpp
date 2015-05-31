#include "pacman.h"




// Constants
const uint16_t INTELLIGENCE_BLINKY = 90;	// intelligence for each ghost
const uint16_t INTELLIGENCE_PINKY = 60;
const uint16_t INTELLIGENCE_INKY = 30;
const uint16_t INTELLIGENCE_CLYDE = 0;
const uint16_t INIT_UP_DOWN = 0;		// initial up/down cycles before the ghost will be allowed to leave the castle
const uint16_t INIT_UP_DOWN_INKY = 5;
const uint16_t INIT_UP_DOWN_CLYDE = 11;
const float WAIT_IN_MS = 2.0;				// duration of a loop (i.e. minimum time between frames)
const uint16_t START_OFFSET = 10;
const int INITIAL_LIVES = 3;            // number of times the player must die to get the "game over"
const int RED = 2;  // color red for init text

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
static void stop_all(uint16_t stop, Pacman *pacman, Ghost **ghost_array) {
	if(stop) {
		ghost_array[0]->set_stop(true);
		ghost_array[1]->set_stop(true);
		ghost_array[2]->set_stop(true);
		ghost_array[3]->set_stop(true);
		pacman->set_stop(true);
		stop_moving = 1;
		
	} else {
		ghost_array[0]->set_stop(false);
		ghost_array[1]->set_stop(false);
		ghost_array[2]->set_stop(false);
		ghost_array[3]->set_stop(false);
		pacman->set_stop(false);
		stop_moving = 0;
	}
}

// SDL event loop: handle keyboard input events, and others
static int eventloop(Pacman *pacman, Ghost **ghost_array) {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(!pacman->is_dying() && !pause) {
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
			    screen->toggleFullscreen();				
			}
			if(event.key.keysym.sym == SDLK_p) {
				if(!pacman->is_dying()) {
					pause = (pause == 0) ? 1 : 0;
				}
			}
			if(event.key.keysym.sym == SDLK_k) {
				ghost_array[0]->set_hunter(Figur::NONE);
				ghost_array[1]->set_hunter(Figur::NONE);
				ghost_array[2]->set_hunter(Figur::NONE);
				ghost_array[3]->set_hunter(Figur::NONE);
			}
			if(event.key.keysym.sym == SDLK_v) {
				ghost_array[0]->set_hunter(Figur::PACMAN);
				ghost_array[1]->set_hunter(Figur::PACMAN);
				ghost_array[2]->set_hunter(Figur::PACMAN);
				ghost_array[3]->set_hunter(Figur::PACMAN);
			}
			if(event.key.keysym.sym == SDLK_b) {
				if(ghost_array[0]->get_hunter() == Figur::PACMAN)
					ghost_array[0]->blink();
				if(ghost_array[1]->get_hunter() == Figur::PACMAN)
					ghost_array[1]->blink();
				if(ghost_array[2]->get_hunter() == Figur::PACMAN)
					ghost_array[2]->blink();
				if(ghost_array[3]->get_hunter() == Figur::PACMAN)
					ghost_array[3]->blink();
			}
			break;
		case SDL_QUIT:
			return 0;
		}		
	}
	return 1;
}


// main function, contains the game loop
int main() {
	SDL_Surface *hintergrund;
	SDL_Surface *score;
	TTF_Font *font, *smallFont;
	SDL_Color textweiss = {255, 255, 255, 0};
	int currentScore = 0, lastScore = 0;
	int loop = 1;
	int start_offset = START_OFFSET;
	float startTicks;
	float lastTickstemp;
	float ms = 1.0;
	float animation_counter = 0;
	srand((unsigned int)time(0)); // init randomize
	
	// create the window 
	screen = new Screen();
	if(screen->occuredInitSdlError() == EXIT_FAILURE)
		return EXIT_FAILURE;

	//create the labyrinth
	Labyrinth *labyrinth = new Labyrinth(screen);
	

	// create an instance of pacman
	Pacman *pacman = new Pacman(310, 338, screen, labyrinth, INITIAL_LIVES);
	
	// init ghosts
	Ghost *blinky = new Ghost(310, 173, INTELLIGENCE_BLINKY, 
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
    hintergrund = LoadSurface("/usr/local/share/pacman/gfx/hintergrund2.png");
	
	// initialize TTF so we are able to write texts to the screen
	if(TTF_Init() == -1) {
		return EXIT_FAILURE;
	}
	font = TTF_OpenFont("/usr/local/share/pacman/fonts/Cheapmot.TTF", 20);
	if(!font) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
		return -1;
	}
	smallFont = TTF_OpenFont("/usr/local/share/pacman/fonts/Cheapmot.TTF", 12);
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
   
	screen->draw(hintergrund);
	labyrinth->init_pillen();
	labyrinth->draw_pillen();
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
	startTicks = (float)SDL_GetTicks();
	blinky->set_leader(1);	// Blinky will be the reference sprite for redrawing
	// at first, stop all figures 
	stop_all(true, pacman, ghost_array_ghost); 
	// game loop
	while(loop) {	
		if(start_offset == -1)	
			loop = eventloop(pacman, ghost_array_ghost);
		
		if(animation_counter > 50) {
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
					stop_all(true, pacman, ghost_array_ghost);
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
						start_offset = START_OFFSET;
					}
				}
			}
			labyrinth->pill_animation();
			
			// handle start offset 
			if(start_offset > 0)
				start_offset--;
			else if (start_offset == 0) {
				labyrinth->hideInitText();
				stop_all(false, pacman, ghost_array_ghost);
				start_offset = -1;
		 	}
				
			animation_counter = 0;
		}
		else
			refresh_ghosts = 0;

		animation_counter = animation_counter + ms;
		pacman->check_eat_pills(ghost_array);
		if(labyrinth->getExisitingPills() <= 0) { 
			// init new level
			reset_all(pacman, ghost_array);
			stop_all(true, pacman, ghost_array_ghost);
			screen->draw(hintergrund);
			labyrinth->initNewLevel();
			start_offset = START_OFFSET;
			continue;
		}
		if(labyrinth->cnt_hunting_mode == 0) {
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
		}
		else if(labyrinth->cnt_hunting_mode > 0 && !pause && labyrinth->cnt_sleep <= 0) {
			labyrinth->cnt_hunting_mode--;
			if(labyrinth->cnt_hunting_mode==2000) {
				blinky->blink();
				pinky->blink();
				inky->blink();
				clyde->blink();
			}
		}

		lastScore = currentScore;
		currentScore = labyrinth->getScore();
		if (lastScore < 10000 && currentScore >= 10000) {
			pacman->addLives(1);
		}

		if (moving()) {
		    // redraw background and pills, but only if the reference ghost has moved
		    screen->draw(hintergrund);
		    labyrinth->draw_pillen();
			labyrinth->compute_score();
			labyrinth->drawInitText();
			screen->draw(score, 530, 30);
			pacman->drawLives();
			labyrinth->drawSmallScore();
			labyrinth->drawInfoFruit();
			labyrinth->setFruit();
			pacman->animate();
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

		if(moving()) {
			pacman->draw();
			blinky->draw();
			pinky->draw();
			inky->draw();
			clyde->draw();
		    labyrinth->draw_blocks();
			screen->Refresh();
			if(pacman->touch(ghost_array)  && !pacman->is_dying()) {
				stop_all(true, pacman, ghost_array_ghost);
				pacman->set_dying(10);  
			}
		}
				
		// determine the correct game speed
		lastTickstemp = startTicks;
		startTicks = (float)SDL_GetTicks();
		lastTickstemp = startTicks - lastTickstemp;
		if(lastTickstemp <= WAIT_IN_MS) {
		  	SDL_Delay((int)(WAIT_IN_MS - lastTickstemp));
			ms = (float)(lastTickstemp/WAIT_IN_MS);
		}
		else 
			ms = (float)(lastTickstemp/WAIT_IN_MS);
		
		// and move all figures
		if ((ms > 0.0) && !pause && labyrinth->cnt_sleep <= 0) {
			pacman->move(moving(), ms);
			blinky->move(moving(), ms);
			pinky->move(moving(), ms);
			inky->move(moving(), ms);
			clyde->move(moving(), ms);
		} else if (moving()) {
			pacman->addUpdateRect();
			blinky->addUpdateRect();
			pinky->addUpdateRect();
			inky->addUpdateRect();
			clyde->addUpdateRect();
		}

		if (labyrinth->cnt_sleep > 0 && !pause) {
			labyrinth->cnt_sleep--;
			if (labyrinth->cnt_sleep == 0) {
				labyrinth->hideSmallScore();
				pacman->setVisibility(true);
				blinky->setVisibility(true);
				pinky->setVisibility(true);
				inky->setVisibility(true);
				clyde->setVisibility(true);
			}
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
