#include "pacman.h"




// Constants
const uint16_t INTELLIGENCE_BLINKY = 90;	// intelligence for each ghost
const uint16_t INTELLIGENCE_PINKY = 60;
const uint16_t INTELLIGENCE_INKY = 30;
const uint16_t INTELLIGENCE_CLYDE = 0;
const uint16_t INIT_DIRECTION_LEFT = 0;	// initial direction of movement (only used for Blinky)
const uint16_t INIT_DIRECTION_UP = 1;	// initial direction of movement (used for the other ghosts)
const uint16_t INIT_UP_DOWN = 0;		// initial up/down cycles before the ghost will be allowed to leave the castle
const uint16_t INIT_UP_DOWN_INKY = 5;
const uint16_t INIT_UP_DOWN_CLYDE = 11;
const float WAIT_IN_MS = 2.0;				// duration of a loop (i.e. minimum time between frames)

// initialize static member variables
int Ghost::was_moving_leader = 1;

Screen *screen;

int stop_moving = 0;
int refresh_ghosts = 0;

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
	if(stop_moving) {
		if(refresh_ghosts) {
			return 1;
			}
		else
			return 0;
	}
	else {
		if(Ghost::was_moving_leader) 
			return 1;
		else
			return 0;
	}
}

/* stop all figures */
static void stop_all(uint16_t stop, Pacman *pacman, Ghost *blinky, Ghost *pinky, Ghost *inky, Ghost *clyde) {
	static float speed_ghosts;
	static float speed_pacman;
	if(stop) {
		speed_ghosts = blinky->get_speed();
		speed_pacman = pacman->get_speed();
		blinky->set_speed(0);
		pinky->set_speed(0);
		inky->set_speed(0);
		clyde->set_speed(0);
		pacman->set_speed(0);
		stop_moving = 1;
		
	} else {
		blinky->set_speed(speed_ghosts);
		pinky->set_speed(speed_ghosts);
		inky->set_speed(speed_ghosts);
		clyde->set_speed(speed_ghosts);
		pacman->set_speed(speed_pacman);
		stop_moving = 0;
	}
}


/* compute the score */
static void compute_score(SDL_Surface *punkte, char *char_punktestand, int int_punktestand, TTF_Font *font, SDL_Color *textgelb) {
	static int punktestand;
	if((punktestand != int_punktestand) || moving()) {
		sprintf(char_punktestand, "%d", int_punktestand * 10);
		punkte = TTF_RenderText_Solid(font, char_punktestand, (*textgelb));
		screen->draw_dynamic_content(punkte, 530, 60);
		punktestand = int_punktestand;
	}	
}

// SDL event loop: handle keyboard input events, and others
static int eventloop(Pacman *pacman, Ghost *blinky, 
Ghost *pinky, Ghost *inky, Ghost *clyde) {
	static uint16_t pause;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(!pacman->is_dying() && !pause) {
				if(event.key.keysym.sym == SDLK_LEFT)
					pacman->richtung_pre = 0;
				if(event.key.keysym.sym == SDLK_UP) 
					pacman->richtung_pre = 1; 
				if(event.key.keysym.sym == SDLK_RIGHT) 
					pacman->richtung_pre = 2;
				if(event.key.keysym.sym == SDLK_DOWN) 
					pacman->richtung_pre = 3; 
			}
			if(event.key.keysym.sym == SDLK_f) {
			    screen->toggleFullscreen();				
			}
			if(event.key.keysym.sym == SDLK_p) {
				if(!pacman->is_dying()) {
					if(!stop_moving) {
						stop_all(true, pacman, blinky, pinky, inky, clyde);  
						pause = 1;
					}
					else {
						stop_all(false, pacman, blinky, pinky, inky, clyde);
						pause = 0;
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
int main() {
	SDL_Surface *hintergrund;
	SDL_Surface *punkte, *score;
	TTF_Font *font;
	SDL_Color textgelb = {255, 247, 11, 0};
	SDL_Color textweiss = {255, 255, 255, 0};
	char char_punktestand[8] = "0";
	int int_punktestand = 0;
	int loop = 1;
	int start_offset = 10;
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
	Pacman *pacman = new Pacman(310, 338);
	pacman->set_labyrinth(labyrinth);
	pacman->set_screen(screen);

	// init ghosts
	Ghost *blinky = new Ghost(310, 173, INTELLIGENCE_BLINKY, 
	                          INIT_DIRECTION_LEFT, INIT_UP_DOWN, Ghost::BLINKY);
	blinky->set_labyrinth(labyrinth);
	blinky->set_screen(screen);
	
	Ghost *pinky = new Ghost(310, 222, INTELLIGENCE_PINKY, 
	                         INIT_DIRECTION_UP, INIT_UP_DOWN, Ghost::PINKY);
	pinky->set_labyrinth(labyrinth);
	pinky->set_screen(screen);
	
	Ghost *inky = new Ghost(280, 222, INTELLIGENCE_INKY, 
	                        INIT_DIRECTION_UP, INIT_UP_DOWN_INKY, Ghost::INKY);
	inky->set_labyrinth(labyrinth);
	inky->set_screen(screen);
	
	Ghost *clyde = new Ghost(340, 222, INTELLIGENCE_CLYDE, 
	                         INIT_DIRECTION_UP, INIT_UP_DOWN_CLYDE, Ghost::CLYDE);
	clyde->set_labyrinth(labyrinth);
	clyde->set_screen(screen);
	
	Figur *ghost_array[4] = {blinky, pinky, inky, clyde};
	

	// initialize background graphic
    hintergrund = LoadSurface("/usr/local/share/pacman/gfx/hintergrund2.png");
	
	// initialize TTF so we are able to write texts to the screen
	if(TTF_Init() == -1) {
		return EXIT_FAILURE;
	}
	//font = TTF_OpenFont("comicbd.ttf", 20);
	font = TTF_OpenFont("/usr/local/share/pacman/fonts/Cheapmot.TTF", 20);
	if(!font) {
		printf("Unable to open TTF font: %s\n", TTF_GetError());
		return -1;
	}
	punkte = TTF_RenderText_Solid(font, char_punktestand, textgelb);
	if(punkte == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	score = TTF_RenderText_Solid(font, "Score", textweiss);
	if(score == NULL) {
		printf("Unable to render text: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
        
	screen->draw(hintergrund);
	labyrinth->init_pillen();
	labyrinth->draw_pillen();
	pacman->draw();
	blinky->draw();
	pinky->draw();
	inky->draw();
	clyde->draw();
	
	screen->draw(score, 530, 30);
	screen->draw(punkte, 530, 60);
	screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
	screen->Refresh();
	startTicks = (float)SDL_GetTicks();
	blinky->set_leader(1);	// Blinky will be the reference sprite for redrawing
	// at first, stop all figures 
	stop_all(true, pacman, blinky, pinky, inky, clyde); 

	// game loop
	while(loop) {	
		if(start_offset == -1)	
			loop = eventloop(pacman, blinky, pinky, 
			inky, clyde);
		
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
					pacman->reset();
					blinky->reset();
					pinky->reset();
					inky->reset();
					clyde->reset();
					stop_all(true, pacman, blinky, pinky, inky, clyde);
					start_offset = 10;
				}
			}
			labyrinth->pill_animation();
			
			// handle start offset 
			if(start_offset > 0)
				start_offset--;
			else if (start_offset == 0) {
				stop_all(false, pacman, blinky, pinky, inky, clyde);
				start_offset = -1;
		 	}
				
			animation_counter = 0;
		}
		else
			refresh_ghosts = 0;

		animation_counter = animation_counter + ms;
		pacman->check_eat_pills(&int_punktestand, ghost_array);
		if(labyrinth->cnt_hunting_mode == 0) {
			blinky->set_hunter(Figur::GHOST);
			pinky->set_hunter(Figur::GHOST);
			inky->set_hunter(Figur::GHOST);
			clyde->set_hunter(Figur::GHOST);
			labyrinth->cnt_hunting_mode--;
		}
		else if(labyrinth->cnt_hunting_mode > 0) {
			labyrinth->cnt_hunting_mode--;
			if(labyrinth->cnt_hunting_mode==2000) {
				blinky->blink();
				pinky->blink();
				inky->blink();
				clyde->blink();
			}
		}
			
		if (moving()) {
		    // redraw background and pills, but only if Blinky (=reference ghost for movement) has moved
		    screen->draw(hintergrund);
		    labyrinth->draw_pillen();
			compute_score(punkte, char_punktestand, int_punktestand, font, &textgelb); 
			screen->draw(score, 530, 30);
			pacman->animate();
		}
		//pacman->animate();
			
	  	// if pacman stops, please set it to "normal"
		if(pacman->is_pacman_stopped() && !pacman->is_dying()) {
			if(pacman->get_richtung() == 0)
		  		pacman->left_pic(0); 
		  	if(pacman->get_richtung() == 1) 
		   		pacman->up_pic(0); 
		 	if(pacman->get_richtung() == 2) 
		    	pacman->right_pic(0); 
		  	if(pacman->get_richtung() == 3) 
	    		pacman->down_pic(0); 
			pacman->parking();
		}		

		//pacman->draw();
		if(moving()) {
			pacman->draw();
			blinky->draw();
			pinky->draw();
			inky->draw();
			clyde->draw();
		    labyrinth->draw_blocks();
			screen->Refresh();
			if(pacman->touch(ghost_array)  && !pacman->is_dying()) {
				stop_all(true, pacman, blinky, pinky, inky, clyde);
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
		pacman->move(moving(), ms);
		blinky->move(moving(), pacman, ms);
		pinky->move(moving(), pacman, ms);
		inky->move(moving(), pacman, ms);
		clyde->move(moving(), pacman, ms);
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
	delete screen;

	return EXIT_SUCCESS;
}
