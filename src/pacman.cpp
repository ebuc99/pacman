#include "pacman.h"




// Constants
const float PACMAN_V_FAST = 0.2f;			// pacman's speed when not eating
const float PACMAN_V_SLOW = 0.18f; 			// pacman's speed while eating
const float GHOSTS_V = 0.18f;				// normal speed of the ghosts
const unsigned short int WECHSEL_RATE = 7;		// load a new image for pacman after a movement of this number of pixels
const unsigned short int INTELLIGENCE_BLINKY = 90;	// intelligence for each ghost
const unsigned short int INTELLIGENCE_PINKY = 60;
const unsigned short int INTELLIGENCE_INKY = 30;
const unsigned short int INTELLIGENCE_CLYDE = 0;
const unsigned short int INIT_DIRECTION_LEFT = 0;	// initial direction of movement (only used for Blinky)
const unsigned short int INIT_DIRECTION_UP = 1;	// initial direction of movement (used for the other ghosts)
const unsigned short int INIT_UP_DOWN = 0;		// initial up/down cycles before the ghost will be allowed to leave the castle
const unsigned short int INIT_UP_DOWN_INKY = 5;
const unsigned short int INIT_UP_DOWN_CLYDE = 11;
const float WAIT_IN_MS = 2.0;				// duration of a loop (i.e. minimum time between frames)
const unsigned short int ANZAHL_SCHIENEN = 50;	// number of rails
const unsigned short int ANZAHL_SCHIENEN_PILLE = 37; // number of pill-filled rails
const unsigned short int ANZAHL_PILLEN = 246;		// number of pills

// initialize static member variables
int Ghost::was_moving_leader = 1;

// define global Variables
enum Richtung {
	Links,
	Oben,
	Rechts,
	Unten
};
 

Pille pillen[ANZAHL_PILLEN];
Screen *screen;
int fullscreen = 0;
int stop_moving = 0;
int refresh_ghosts = 0;


// rail initialization - quick and dirty, should be done in a more elegant way
const Schiene s0(207, 338, 412, 338);
const Schiene s1(207, 37, 207, 380);
const Schiene s2(138, 92, 480, 92);
const Schiene s3(412, 37, 412, 380);
const Schiene s4(138, 37, 290, 37);
const Schiene s5(330, 37, 480, 37);
const Schiene s6(138, 37, 138, 133);
const Schiene s7(480, 37, 480, 133);
const Schiene s8(138, 133, 207, 133);
const Schiene s9(412, 133, 480, 133);
const Schiene s10(290, 37, 290, 92);
const Schiene s11(330, 37, 330, 92);
const Schiene s12(249, 92, 249, 133);
const Schiene s13(371, 92, 371, 133);
const Schiene s14(249, 133, 290, 133);
const Schiene s15(330, 133, 371, 133);
const Schiene s16(290, 133, 290, 173);
const Schiene s17(330, 133, 330, 173);
const Schiene s18(249, 173, 371, 173);
const Schiene s19(249, 173, 249, 298);
const Schiene s20(371, 173, 371, 298);
const Schiene s21(249, 257, 371, 257);
const Schiene s22(100, 215, 249, 215);
const Schiene s23(371, 215, 515, 215);
const Schiene s24(138, 298, 290, 298);
const Schiene s25(330, 298, 480, 298);
const Schiene s26(290, 298, 290, 338);
const Schiene s27(330, 298, 330, 338);
const Schiene s28(138, 298, 138, 338);
const Schiene s29(480, 298, 480, 338);
const Schiene s30(138, 338, 166, 338);
const Schiene s31(452, 338, 480, 338);
const Schiene s32(166, 338, 166, 380);
const Schiene s33(452, 338, 452, 380);
const Schiene s34(138, 380, 207, 380);
const Schiene s35(412, 380, 480, 380);
const Schiene s36(138, 380, 138, 420);
const Schiene s37(480, 380, 480, 420);
const Schiene s38(138, 420, 480, 420);
const Schiene s39(249, 338, 249, 380);
const Schiene s40(371, 338, 371, 380);
const Schiene s41(249, 380, 290, 380);
const Schiene s42(330, 380, 371, 380);
const Schiene s43(290, 380, 290, 420);
const Schiene s44(330, 380, 330, 420);

// ghost castle
const Schiene s45(310, 174, 310, 222);
const Schiene s46(280, 222, 309, 222);
const Schiene s47(311, 222, 340, 222);
const Schiene s48(280, 200, 280, 222);
const Schiene s49(340, 200, 340, 222);

Schiene ar_s[ANZAHL_SCHIENEN] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44, s45, s46, s47, s48, s49};
Schiene ar_s_pille[ANZAHL_SCHIENEN_PILLE] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44};

// helper functions - maybe move to a separated class?
static int less(int a, int b) {
	if(a < b) 
		return a;
	else 
		return b;
}
static int greater(int a, int b) {
	if(a > b) 
		return a;
	else 
		return b;

}

static SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1) {
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filename);
	if(!temp) {
		printf("Unable to load image: %s\n", IMG_GetError());
		exit(-1);
	}
	if(transparent_color != -1)
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, transparent_color, transparent_color, transparent_color));
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
static void stop_all(unsigned short int stop, Pacman *pacman, Ghost *blinky, Ghost *pinky, Ghost *inky, Ghost *clyde) {
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


/* this function registers the ghosts' graphics for redrawing, but only if they have changed */
static void AddUpdateRects_ghost(Ghost *ghost_l) {
	if((ghost_l->get_richtung() == 0) || (ghost_l->get_richtung() == 1))
		screen->AddUpdateRects(ghost_l->x, ghost_l->y, (ghost_l->ghost_sf->w + abs(ghost_l->x - ghost_l->last_x)), (ghost_l->ghost_sf->h + abs(ghost_l->y - ghost_l->last_y)));
	else
		screen->AddUpdateRects((ghost_l->x - abs(ghost_l->x - ghost_l->last_x)), (ghost_l->y - abs(ghost_l->y - ghost_l->last_y)), ghost_l->ghost_sf->w, ghost_l->ghost_sf->h);
}


/* redraw background, but only if Blinky (=reference ghost for movement) has moved */
static void draw_hintergrund(SDL_Surface *hintergrund, SDL_Surface *screen) {
	if(moving()) {
		SDL_BlitSurface(hintergrund, NULL, screen, NULL);
 	}
}

/* initially, throw some pills across the level */
static void init_pillen() {
	int m = 0;
	int i_ar_pille_x[26] = {148,162,176,190,204,217,231,245,259,273,287,300,314,327,340,354,368,381,395,409,422,436,449,462,476,490};
	int i_ar_pille_y[29] = {47,61,75,89,102,116,130,143,157,170,184,197,211,225,239,252,266,280,294,308,321,335,348,362,376,390,404,417,430};
		for(int i = 0; i < 26; i++) {
			for(int j = 0; j < 29; j++) {
				for(int k = 0; k < ANZAHL_SCHIENEN_PILLE; k++) {
					if(((ar_s_pille[k].x1 <= (i_ar_pille_x[i] - 10)) && (ar_s_pille[k].x2 >= (i_ar_pille_x[i] - 10)) && (ar_s_pille[k].y1 == (i_ar_pille_y[j] - 10)) && (ar_s_pille[k].y2 == (i_ar_pille_y[j] - 10))) || ((ar_s_pille[k].x1 == (i_ar_pille_x[i] - 10)) && (ar_s_pille[k].x2 == (i_ar_pille_x[i] - 10)) && (ar_s_pille[k].y1 <= (i_ar_pille_y[j] - 10)) && (ar_s_pille[k].y2 >= (i_ar_pille_y[j] - 10)))) {
						pillen[m].x = i_ar_pille_x[i];
						pillen[m].y = i_ar_pille_y[j];
						pillen[m].sichtbar = 1;
						if(((i_ar_pille_x[i] == 148) && (i_ar_pille_y[j] == 75)) || ((i_ar_pille_x[i] == 490) && (i_ar_pille_y[j] == 75)) || ((i_ar_pille_x[i] == 148) && (i_ar_pille_y[j] == 348)) || ((i_ar_pille_x[i] == 490) && (i_ar_pille_y[j] == 348)))
							pillen[m].superpille = 1;
						else
							pillen[m].superpille = 0;
						m++;
						break;
					}
				}
			}
		}
}

/* draw pills, but only if Blinky has moved */
static void draw_pillen(SDL_Surface *pille, SDL_Surface *superpille) {
	if(moving()){
		SDL_Rect dest;
		for(int i = 0; i < ANZAHL_PILLEN; i++) {
			if(pillen[i].sichtbar && !pillen[i].superpille) {
				dest.x = pillen[i].x;
				dest.y = pillen[i].y;
				SDL_BlitSurface(pille, NULL, screen->screen_sf, &dest);
			}
			if(pillen[i].sichtbar && pillen[i].superpille) {
				dest.x = pillen[i].x - 4;
				dest.y = pillen[i].y - 4;
				SDL_BlitSurface(superpille, NULL, screen->screen_sf, &dest);
				screen->AddUpdateRects(dest.x , dest.y, superpille->w, superpille->h);
			}
		}
	}
}

// redraw static content, which only has to be redrawn sometimes
static void draw_static_content(SDL_Surface *surface, int x, int y, int force = 0) {
	static unsigned short int  temp_force = 0;
	if(force)
		temp_force = force;
	if(moving() && temp_force) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, screen->screen_sf, &dest);
		screen->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
		temp_force = 0;
	}
}

// especially for drawing the score
static void draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	if(moving()) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, screen->screen_sf, &dest);
		screen->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
	}
}

/* compute the score */
static void compute_score(SDL_Surface *punkte, char *char_punktestand, int int_punktestand, TTF_Font *font, SDL_Color *textgelb) {
	static int punktestand;
	if((punktestand != int_punktestand) || moving()) {
		sprintf(char_punktestand, "%d", int_punktestand * 10);
		punkte = TTF_RenderText_Solid(font, char_punktestand, (*textgelb));
		draw_dynamic_content(punkte, 530, 60);
		punktestand = int_punktestand;
	}	
}

/* collision handling pacman <-> pills */
static void check_pillen(Pacman *pacman, int *punktestand) {
	if(pacman->was_moving()){
		static int cnt_slow;	// number of loops that pacman will stay slow
		for(int i = 0; i < ANZAHL_PILLEN; i++) {
			if(pillen[i].sichtbar && ((pillen[i].x - 10) >= less(pacman->x,pacman->last_x)) && ((pillen[i].x - 10) <= greater(pacman->x,pacman->last_x)) && ((pillen[i].y - 10) >= less(pacman->y,pacman->last_y)) && ((pillen[i].y - 10) <= greater(pacman->y,pacman->last_y))) {
				cnt_slow = 15;
				pillen[i].sichtbar = 0;
				pacman->set_speed(PACMAN_V_SLOW);
				(*punktestand)++;
				break;
			}	
		}	
		// only if pacman really has moved
		if(cnt_slow > 0)
			cnt_slow--; 
		if(cnt_slow <= 0)
			pacman->set_speed(PACMAN_V_FAST); // wieder schnell
	}
}

/* reset */
static void reset(Pacman *pacman, Ghost *blinky, Ghost *pinky, Ghost *inky, Ghost *clyde) {
	screen->AddUpdateRects(blinky->x, blinky->y, blinky->ghost_sf->w, blinky->ghost_sf->h);
	screen->AddUpdateRects(pinky->x, pinky->y, pinky->ghost_sf->w, pinky->ghost_sf->h);
	screen->AddUpdateRects(inky->x, inky->y, inky->ghost_sf->w, inky->ghost_sf->h);
	screen->AddUpdateRects(clyde->x, clyde->y, clyde->ghost_sf->w, clyde->ghost_sf->h);
	screen->AddUpdateRects(pacman->x, pacman->y, pacman->pacman_sf->w, pacman->pacman_sf->h);
	pacman->reset();
	blinky->reset();
	pinky->reset();
	inky->reset();
	clyde->reset();
}

/* collision handling pacman <-> ghosts */
static int check_collision(Pacman *pacman, Ghost **ghost_array) {
	unsigned short int x_left_pacman = pacman->x;
	unsigned short int y_up_pacman = pacman->y;
	unsigned short int x_right_pacman = pacman->x + pacman->pacman_sf->w;
	unsigned short int y_down_pacman = pacman->y + pacman->pacman_sf->h;
	
	if(moving()) {
	    for(int i = 0; i <= 3; i++) {
	    	unsigned short int x_real_ghost = ghost_array[i]->x + (int)(ghost_array[i]->ghost_sf->w * 0.5);
			unsigned short int y_real_ghost = ghost_array[i]->y + (int)(ghost_array[i]->ghost_sf->h * 0.5);
			if((x_real_ghost >= x_left_pacman) && (x_real_ghost <= x_right_pacman) && (y_real_ghost >= y_up_pacman) && (y_real_ghost <= y_down_pacman))
				return 1;
		}
	}			
	return 0;
}

/* Bewege pacman */
static void move_pacman(Pacman *pacman, float ms) {
	if(Ghost::was_moving_leader || (stop_moving && moving())){
		screen->AddUpdateRects(pacman->x, pacman->y, pacman->pacman_sf->w, pacman->pacman_sf->h);
	}
	pacman->move_on_rails(ms, ANZAHL_SCHIENEN, ar_s);
}

/* move pacman on the rails, according to it's speed and direction */
static void move_ghosts(Ghost *ghost_l, Pacman *pacman, float(ms)) {
	if(ghost_l->was_moving() || (stop_moving && moving())){
		AddUpdateRects_ghost(ghost_l);	
	}
	
	ghost_l->move_on_rails(pacman, ms, ANZAHL_SCHIENEN, ar_s);
}


// SDL event loop: handle keyboard input events, and others
static int eventloop(SDL_Surface *hintergrund, Pacman *pacman, Ghost *blinky, 
Ghost *pinky, Ghost *inky, Ghost *clyde, SDL_Surface *score) {
	static unsigned short int pause;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(!pacman->is_dying && !pause) {
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
				fullscreen = !fullscreen;
    			SDL_Surface* newScreen;
				if(fullscreen)
      				newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_FULLSCREEN);
				else
      				newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE);
    			if (NULL != newScreen) {  // successful? NULL indicates failure
      				screen->screen_sf = newScreen;  // take it, but do not dispose of the old screen (says SDL documentation)
					screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
					// no Refresh() here, because at this moment nothing has been drawn to the new screen
    			}
			}
			if(event.key.keysym.sym == SDLK_p) {
				if(!pacman->is_dying) {
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
int main(int argc, char *argv[]) {
	SDL_Surface *hintergrund, *pille;
	SDL_Surface *superpille[5];
	SDL_Surface *punkte, *score;
	TTF_Font *font;
	SDL_Color textgelb = {255, 247, 11, 0};
	SDL_Color textweiss = {255, 255, 255, 0};
	char char_punktestand[8] = "0";
	int int_punktestand = 0;
	int ghost_change = 0;
	int loop = 1;
	int ct_pm = 0;
	unsigned short int die_counter = 0;
	unsigned short int pille_counter = 0;
	short int start_offset = 10;
	float startTicks;
	float lastTickstemp;
	float ms = 1.0;
	float wechsel_counter = 0;
	
	// create the window 
	screen = new Screen();
	if(screen->sdl_init_error == EXIT_FAILURE)
		return EXIT_FAILURE;

	//create the labyrinth
	Labyrinth *labyrinth = new Labyrinth(screen->screen_sf);
	

	// create an instance of pacman
	Pacman *pacman = new Pacman(310, 338, PACMAN_V_FAST, WECHSEL_RATE);

	// init ghosts
	Ghost *blinky = new Ghost(310, 173, GHOSTS_V, INTELLIGENCE_BLINKY, 
	                          INIT_DIRECTION_LEFT, INIT_UP_DOWN, Ghost::BLINKY);
	Ghost *pinky = new Ghost(310, 222, GHOSTS_V, INTELLIGENCE_PINKY, 
	                         INIT_DIRECTION_UP, INIT_UP_DOWN, Ghost::PINKY);
	Ghost *inky = new Ghost(280, 222, GHOSTS_V,  INTELLIGENCE_INKY, 
	                        INIT_DIRECTION_UP, INIT_UP_DOWN_INKY, Ghost::INKY);
	Ghost *clyde = new Ghost(340, 222, GHOSTS_V, INTELLIGENCE_CLYDE, 
	                         INIT_DIRECTION_UP, INIT_UP_DOWN_CLYDE, Ghost::CLYDE);
	Ghost *ghost_array[4] = {blinky, pinky, inky, clyde};
	

	/* Grafiken initialisieren */
    hintergrund = LoadSurface("/usr/local/share/pacman/gfx/hintergrund2.png");
    pille = LoadSurface("/usr/local/share/pacman/gfx/pille.png");	
	superpille[0] = LoadSurface("/usr/local/share/pacman/gfx/superpille_1.png", 0);
	superpille[1] = LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	superpille[2] = LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	superpille[3] = LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	superpille[4] = LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	
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
        
	draw_hintergrund(hintergrund, screen->screen_sf);
	init_pillen();
	draw_pillen(pille, superpille[pille_counter]);
	pacman->draw(screen->screen_sf);
	blinky->draw(screen->screen_sf, moving());
	pinky->draw(screen->screen_sf, moving());
	inky->draw(screen->screen_sf, moving());
	clyde->draw(screen->screen_sf, moving());
	draw_dynamic_content(score, 530, 30);
	draw_dynamic_content(punkte, 530, 60);
	screen->AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
	screen->Refresh(moving());
	startTicks = (float)SDL_GetTicks();
	blinky->set_leader(1);	// Blinky will be the reference sprite for redrawing
	// at first, stop all figures 
	stop_all(true, pacman, blinky, pinky, inky, clyde); 

	// game loop
	while(loop) {	
		if(start_offset == -1)	
			loop = eventloop(hintergrund, pacman, blinky, pinky, 
			inky, clyde, score);
		
		if(wechsel_counter > 50) {
			// ghost animations
			refresh_ghosts = 1;
			if(ghost_change)
				ghost_change = 0;
			else
				ghost_change = 1;
			blinky->animation(ghost_change);
			pinky->animation(ghost_change);
			inky->animation(ghost_change);
			clyde->animation(ghost_change);
			
			//
			if(pacman->is_dying) {
				if(pacman->is_dying > 1)
					pacman->is_dying--;
				else {
					pacman->die_pic(die_counter);
					die_counter++;
					if(die_counter == 13) {
						pacman->is_dying = 0;
						reset(pacman, blinky, pinky, inky, clyde);
						stop_all(true, pacman, blinky, pinky, inky, clyde);
						ct_pm = 0;
						start_offset = 10;
						die_counter = 0;
					}
				}
			}
			
			if(pille_counter == 4)
				pille_counter = 0;
			else
				pille_counter++;			
			
			// handle start offset 
			if(start_offset > 0)
				start_offset--;
			else if (start_offset == 0) {
				stop_all(false, pacman, blinky, pinky, inky, clyde);
				start_offset = -1;
		 	}
				
			wechsel_counter = 0;
		}
		else
			refresh_ghosts = 0;
		wechsel_counter = wechsel_counter + ms;
		
		draw_hintergrund(hintergrund, screen->screen_sf);
		check_pillen(pacman, &int_punktestand);
		compute_score(punkte, char_punktestand, int_punktestand, font, &textgelb); 
		draw_static_content(score, 530, 30);
		draw_pillen(pille, superpille[pille_counter]);
	
		if(pacman->wechsel()) {
			if(pacman->get_richtung() == 0)
		  		pacman->left_pic(ct_pm); 
		  	if(pacman->get_richtung() == 1) 
		   		pacman->up_pic(ct_pm); 
		  	if(pacman->get_richtung() == 2) 
		    	pacman->right_pic(ct_pm); 
		  	if(pacman->get_richtung() == 3) 
	    		pacman->down_pic(ct_pm); 
		  	ct_pm++;
		}
		if(ct_pm > 3)
			ct_pm = 0;
			
	  	// if pacman stops, please set it to "normal"
		if(pacman->is_pacman_stopped() && !pacman->is_dying) {
			ct_pm = 0;
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
		
		blinky->draw(screen->screen_sf, moving());
		pinky->draw(screen->screen_sf, moving());
		inky->draw(screen->screen_sf, moving());
		clyde->draw(screen->screen_sf, moving());
		pacman->draw (screen->screen_sf);
		if(moving())
			labyrinth->draw_blocks();
		screen->Refresh(moving());
		if(check_collision(pacman, ghost_array) && !pacman->is_dying) {
			stop_all(true, pacman, blinky, pinky, inky, clyde);
			pacman->is_dying = 10;  
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
		move_pacman(pacman, ms);
		move_ghosts(blinky, pacman, ms);
		move_ghosts(pinky, pacman, ms);
		move_ghosts(inky, pacman, ms);
		move_ghosts(clyde, pacman, ms);
	}
	
	// clean up SDL
    TTF_CloseFont(font);
    TTF_Quit();
	SDL_FreeSurface(pille);
	SDL_FreeSurface(hintergrund);

	delete pacman;
	delete blinky;
	delete pinky;
	delete inky;
	delete clyde;

	return EXIT_SUCCESS;
}
