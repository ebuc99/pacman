#include "pacman.h"




/* Konstanten */
const unsigned short int UPDATERECTS = 200; 		// Anzahl der Rechtecke, die maximal neu gezeichnet werden
const float PACMAN_V_FAST = 0.2f;			// Geschwindigkeit von Pacman ohne zu fressen
const float PACMAN_V_SLOW = 0.18f; 			// Geschwindigkeit von Pacman während des Fressens
const float GHOSTS_V = 0.18f;				// Geschwindigkeit der Geister
const unsigned short int WECHSEL_RATE = 7;		// gibt an, nach wievielen Pixel Bewegung für Pacman ein neues Bild geladen wird (nur für Pacman
							// wichtig
const unsigned short int INTELLIGENCE_BLINKY = 90;	// Intelligenz der Geister							
const unsigned short int INTELLIGENCE_PINKY = 60;
const unsigned short int INTELLIGENCE_INKY = 30;
const unsigned short int INTELLIGENCE_CLYDE = 0;
const unsigned short int INIT_DIRECTION_LEFT = 0;	// initiale Startrichtung links (Blinky)
const unsigned short int INIT_DIRECTION_UP = 1;	// initiale Startrichtung hoch (alle anderen Geister)
const unsigned short int INIT_UP_DOWN = 0;		// wie oft hoch und runter bevor der Geist aus dem Schloss darf
const unsigned short int INIT_UP_DOWN_INKY = 5;
const unsigned short int INIT_UP_DOWN_CLYDE = 11;
const float WAIT_IN_MS = 2.0;				// Dauer eines Schleifensurchlaufs
const unsigned short int ANZAHL_SCHIENEN = 50;	// Anzahl der Schienen	
const unsigned short int ANZAHL_SCHIENEN_PILLE = 37; // Anzahl der Schienen mit Pille
const unsigned short int ANZAHL_PILLEN = 246;		// Anzahl der Pillen

// statische Elementvariable definieren
int Ghost::was_moving_leader = 1;

// globale Variablen
enum Richtung {
	Links,
	Oben,
	Rechts,
	Unten
};
 
SDL_Surface *screen;

int rect_num = 0;

SDL_Rect rects[UPDATERECTS];
Pille pillen[ANZAHL_PILLEN];
int fullscreen = 0;
int stop_moving = 0;
int refresh_ghosts = 0;
//int pacman_dying = 0;


// nur zum Test, müßte dann anders initialisiert werden
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

//Ghost Castle
const Schiene s45(310, 174, 310, 222);
const Schiene s46(280, 222, 309, 222);
const Schiene s47(311, 222, 340, 222);
const Schiene s48(280, 200, 280, 222);
const Schiene s49(340, 200, 340, 222);

Schiene ar_s[ANZAHL_SCHIENEN] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44, s45, s46, s47, s48, s49};
Schiene ar_s_pille[ANZAHL_SCHIENEN_PILLE] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44};

/* das folgende sind Hilfsfunktionen, vielleicht in Extraklasse auslagern */
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

static SDL_Surface *LoadSurface(const char *filename, int transparant_color = -1) {
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filename);
	if(!temp) {
		printf("Error IMG_Load: %s\n", IMG_GetError());
		exit(-1);
	}
	if(transparant_color != -1)
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, transparant_color, transparant_color, transparant_color));
	surface = SDL_DisplayFormat(temp);
	if(surface == NULL) {
		printf("Konnte Grafik nicht laden: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
        }
    SDL_FreeSurface(temp);
    return surface;	
}

/* hier wird entschieden, ob was neugezeichnet werden soll */
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

/* alles anhalten */
static void stop_all(unsigned short int stop, Pacman *pacman_l, Ghost *blinky_l, Ghost *pinky_l, Ghost *inky_l, Ghost *clyde_l) {
	static float speed_ghosts;
	static float speed_pacman;
	if(stop) {
		speed_ghosts = blinky_l->get_speed();
		speed_pacman = pacman_l->get_speed();
		blinky_l->set_speed(0);
		pinky_l->set_speed(0);
		inky_l->set_speed(0);
		clyde_l->set_speed(0);
		pacman_l->set_speed(0);
		stop_moving = 1;
	} else {
		blinky_l->set_speed(speed_ghosts);
		pinky_l->set_speed(speed_ghosts);
		inky_l->set_speed(speed_ghosts);
		clyde_l->set_speed(speed_ghosts);
		pacman_l->set_speed(speed_pacman);
		stop_moving = 0;
	}
}


/* diese Funktion meldet die Geistergrafiken nur dann zum neu Zeichnen an, wenn sie sich geändert haben */
static void AddUpdateRects_ghost(SDL_Surface *ghost, Ghost *ghost_l) {
	if((ghost_l->get_richtung() == 0) || (ghost_l->get_richtung() == 1))
		AddUpdateRects(ghost_l->x, ghost_l->y, (ghost->w + abs(ghost_l->x - ghost_l->last_x)), (ghost->h + abs(ghost_l->y - ghost_l->last_y)));
	else
		AddUpdateRects((ghost_l->x - abs(ghost_l->x - ghost_l->last_x)), (ghost_l->y - abs(ghost_l->y - ghost_l->last_y)), ghost->w, ghost->h);
}


/* Hintergrund neu zeichnen, aber nur wenn Blinky (Referenzgeist für die Bewegung) sich bewegt hat */
static void draw_hintergrund(SDL_Surface *hintergrund) {
	if(moving()) {
		SDL_BlitSurface(hintergrund, NULL, screen, NULL);
 	}
}

/* Pillen auf dem Spielfeld verteilen */
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

/* Pillen zeichnen, aber nur wenn Blinky sich bewegt hat */
static void draw_pillen(SDL_Surface *pille, SDL_Surface *superpille) {
	if(moving()){
		SDL_Rect dest;
		for(int i = 0; i < ANZAHL_PILLEN; i++) {
			if(pillen[i].sichtbar && !pillen[i].superpille) {
				dest.x = pillen[i].x;
				dest.y = pillen[i].y;
				SDL_BlitSurface(pille, NULL, screen, &dest);
			}
			if(pillen[i].sichtbar && pillen[i].superpille) {
				dest.x = pillen[i].x - 4;
				dest.y = pillen[i].y - 4;
				SDL_BlitSurface(superpille, NULL, screen, &dest);
				AddUpdateRects(dest.x , dest.y, superpille->w, superpille->h);
			}
		}
	}
}

// zum Zeichnen statischen Inhalts, welches nur manchmal neu gezeichnet werden muß
static void draw_static_content(SDL_Surface *surface, int x, int y, int force = 0) {
	static unsigned short int  temp_force = 0;
	if(force)
		temp_force = force;
	if(moving() && temp_force) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, screen, &dest);
		AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
		temp_force = 0;
	}
}

// zum zeichnen der Punktezählung
static void draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	if(moving()) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, screen, &dest);
		AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
	}
}

/* Punkte berechenen */
static void compute_score(SDL_Surface *punkte, char *char_punktestand, int int_punktestand, TTF_Font *font, SDL_Color *textgelb) {
	static int punktestand;
	if((punktestand != int_punktestand) || moving()) {
		sprintf(char_punktestand, "%d", int_punktestand * 10);
		punkte = TTF_RenderText_Solid(font, char_punktestand, (*textgelb));
		draw_dynamic_content(punkte, 530, 60);
		punktestand = int_punktestand;
	}	
}

/* Kollisionsbehandlung pacman <-> Pillen */
static void check_pillen(Pacman *pacman_l, int *punktestand) {
	if(pacman_l->was_moving()){
		static int cnt_slow;	// Anzahl der Umläufe, die er langsam bleibt
		for(int i = 0; i < ANZAHL_PILLEN; i++) {
			if(pillen[i].sichtbar && ((pillen[i].x - 10) >= less(pacman_l->x,pacman_l->last_x)) && ((pillen[i].x - 10) <= greater(pacman_l->x,pacman_l->last_x)) && ((pillen[i].y - 10) >= less(pacman_l->y,pacman_l->last_y)) && ((pillen[i].y - 10) <= greater(pacman_l->y,pacman_l->last_y))) {
				cnt_slow = 15;
				pillen[i].sichtbar = 0;
				pacman_l->set_speed(PACMAN_V_SLOW);
				(*punktestand)++;
				break;
			}	
		}	
		//bloss wenn er sich real wirklich bewegt hat
		if(cnt_slow > 0)
			cnt_slow--; 
		if(cnt_slow <= 0)
			pacman_l->set_speed(PACMAN_V_FAST); // wieder schnell
	}
}

/* Reset */
static void reset(Pacman *pacman_l, Ghost *blinky_l, Ghost *pinky_l, Ghost *inky_l, Ghost *clyde_l, /*SDL_Surface *pacman,*/ SDL_Surface *ghost) {
	AddUpdateRects(blinky_l->x, blinky_l->y, ghost->w, ghost->h);
	AddUpdateRects(pinky_l->x, pinky_l->y, ghost->w, ghost->h);
	AddUpdateRects(inky_l->x, inky_l->y, ghost->w, ghost->h);
	AddUpdateRects(clyde_l->x, clyde_l->y, ghost->w, ghost->h);
	AddUpdateRects(pacman_l->x, pacman_l->y, pacman_l->pacman->w, pacman_l->pacman->h /*pacman->w, pacman->h*/);
	pacman_l->reset();
	blinky_l->reset();
	pinky_l->reset();
	inky_l->reset();
	clyde_l->reset();
}

/* Kollisionsbehandlung pacman <-> Geister */
static int check_collision(SDL_Surface *pacman, SDL_Surface *ghost, Pacman &rpacman_l, Ghost **ghost_array) {
	unsigned short int x_left_pacman = rpacman_l.x;
	unsigned short int y_up_pacman = rpacman_l.y;
	unsigned short int x_right_pacman = rpacman_l.x + pacman->w;
	unsigned short int y_down_pacman = rpacman_l.y + pacman->h;
	
	if(moving()) {
	    for(int i = 0; i <= 3; i++) {
	    	unsigned short int x_real_ghost = ghost_array[i]->x + (int)(ghost->w * 0.5);
			unsigned short int y_real_ghost = ghost_array[i]->y + (int)(ghost->h * 0.5);
			if((x_real_ghost >= x_left_pacman) && (x_real_ghost <= x_right_pacman) && (y_real_ghost >= y_up_pacman) && (y_real_ghost <= y_down_pacman))
				return 1;
		}
	}			
	return 0;
}


/* Links und rechts beim Mittelgang sind zwei schwarze Rechtecke unter denen pacman und die Geister verschwinden, wenn sie links oder rechts rauslaufen */
static void draw_blocks(void) {
	if(moving()) {
  	SDL_Rect b1, b2;
  	b1.x = 100;
  	b1.y = 215;
  	b1.w = 30;
  	b1.h = 30;
  
 	b2.x = 515;
  	b2.y = 215;
  	b2.w = 30;
  	b2.h = 30;
  
  	SDL_FillRect(screen, &b1, SDL_MapRGB(screen->format, 0, 0, 0));
  	SDL_FillRect(screen, &b2, SDL_MapRGB(screen->format, 0, 0, 0));
	}
}

/* Bewege pacman */
static void move_pacman(SDL_Surface *pacman, Pacman *pacman_l, float ms) {
	if(Ghost::was_moving_leader || (stop_moving && moving())){
		AddUpdateRects(pacman_l->x, pacman_l->y, pacman->w, pacman->h);
	}
	pacman_l->move_on_rails(ms, ANZAHL_SCHIENEN, ar_s);
}

/* Bewege Geist */
static void move_ghosts(SDL_Surface *ghost, Ghost *ghost_l, Pacman *pacman_l, float(ms)) {
	if(ghost_l->was_moving() || (stop_moving && moving())){
		AddUpdateRects_ghost(ghost, ghost_l);	
	}
	
	ghost_l->move_on_rails(pacman_l, ms, ANZAHL_SCHIENEN, ar_s);
}

/* Anmelden von Grafiken zum Neuzeichnen */
void AddUpdateRects(int x, int y, int w, int h) {
	if (x < 0) {
         	w += x;
        	x = 0;
    	}
    	if (y < 0) {
        	h += y;
        	y = 0;
   	}
	if (x + w > screen->w)
       	 	w = screen->w - x;
    	if (y + h > screen->h)
        	h = screen->h - y;
    	if (w <= 0 || h <= 0)
        	return;
    	rects[rect_num].x = x;
    	rects[rect_num].y = y;
    	rects[rect_num].w = w;
    	rects[rect_num].h = h;
    	rect_num++;
}

/* Das Neuzeichnen */
void Refresh() {
	if(moving()){
		SDL_UpdateRects(screen, rect_num, rects);
		rect_num = 0;
	}
}

/* Tastaturereignisse */
static int eventloop(SDL_Surface *hintergrund, Pacman *pacman_l, Ghost *blinky_l, 
Ghost *pinky_l, Ghost *inky_l, Ghost *clyde_l, SDL_Surface *score) {
	static unsigned short int pause;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(!pacman_l->is_dying && !pause) {
				if(event.key.keysym.sym == SDLK_LEFT)
					pacman_l->richtung_pre = 0;
				if(event.key.keysym.sym == SDLK_UP) 
					pacman_l->richtung_pre = 1; 
				if(event.key.keysym.sym == SDLK_RIGHT) 
					pacman_l->richtung_pre = 2;
				if(event.key.keysym.sym == SDLK_DOWN) 
					pacman_l->richtung_pre = 3; 
			}
			if(event.key.keysym.sym == SDLK_f) {
				fullscreen = !fullscreen;
    			SDL_Surface* newScreen;
				if(fullscreen)
      				newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_FULLSCREEN);
				else
      				newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE);
    			if (NULL != newScreen) {  // successful? NULL indicates failure
      				screen = newScreen;  // take it, but do not dispose of the old screen (says SDL documentation)
					AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
					// no Refresh() here, because at this moment nothing has been drawn to the new screen
    			}
			}
			if(event.key.keysym.sym == SDLK_p) {
				if(!pacman_l->is_dying) {
					if(!stop_moving) {
						stop_all(true, pacman_l, blinky_l, pinky_l, inky_l, clyde_l);  
						pause = 1;
					}
					else {
						stop_all(false, pacman_l, blinky_l, pinky_l, inky_l, clyde_l);
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


/* Hauptfunktion mit Spielschleife */
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
	
	// SDL-Initialisierung vornehmen
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("Konnte SDL nicht initialisieren: %s\n",SDL_GetError());
                return EXIT_FAILURE;
        }
	atexit(SDL_Quit);
	screen = SDL_SetVideoMode(680, 512, 24, SDL_HWSURFACE);
        if(screen == 0) {
                printf("Kann Videomodus nicht setzen %s\n",SDL_GetError());
                return EXIT_FAILURE;
        }
	SDL_WM_SetCaption("Pacman", "");

	// init pacman
	Pacman pacman_l(310, 338, PACMAN_V_FAST, WECHSEL_RATE);

	// init ghosts
	Ghost blinky_l(310, 173, GHOSTS_V, INTELLIGENCE_BLINKY, INIT_DIRECTION_LEFT, INIT_UP_DOWN, Ghost::BLINKY);
	Ghost pinky_l(310, 222, GHOSTS_V, INTELLIGENCE_PINKY, INIT_DIRECTION_UP, INIT_UP_DOWN, Ghost::PINKY);
	Ghost inky_l(280, 222, GHOSTS_V,  INTELLIGENCE_INKY, INIT_DIRECTION_UP, INIT_UP_DOWN_INKY, Ghost::INKY);
	Ghost clyde_l(340, 222, GHOSTS_V, INTELLIGENCE_CLYDE, INIT_DIRECTION_UP, INIT_UP_DOWN_CLYDE, Ghost::CLYDE);
	Ghost *ghost_array[4] = {&blinky_l, &pinky_l, &inky_l, &clyde_l};
	

	/* Grafiken initialisieren */
    hintergrund = LoadSurface("/usr/local/share/pacman/gfx/hintergrund2.png");
    pille = LoadSurface("/usr/local/share/pacman/gfx/pille.png");	
	superpille[0] = LoadSurface("/usr/local/share/pacman/gfx/superpille_1.png", 0);
	superpille[1] = LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	superpille[2] = LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	superpille[3] = LoadSurface("/usr/local/share/pacman/gfx/superpille_3.png", 0);
	superpille[4] = LoadSurface("/usr/local/share/pacman/gfx/superpille_2.png", 0);
	
	// TTF initialisieren
	if(TTF_Init() == -1) {
		return EXIT_FAILURE;
	}
	//font = TTF_OpenFont("comicbd.ttf", 20);
	font = TTF_OpenFont("/usr/local/share/pacman/fonts/Cheapmot.TTF", 20);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		return -1;
	}
	punkte = TTF_RenderText_Solid(font, char_punktestand, textgelb);
	if(punkte == NULL) {
		printf("Fehler beim Rendern von TTF: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	score = TTF_RenderText_Solid(font, "Score", textweiss);
	if(score == NULL) {
		printf("Fehler beim Rendern von TTF: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
        
	draw_hintergrund(hintergrund);
	init_pillen();
	draw_pillen(pille, superpille[pille_counter]);
	pacman_l.draw(screen);
	blinky_l.draw(screen, moving());
	pinky_l.draw(screen, moving());
	inky_l.draw(screen, moving());
	clyde_l.draw(screen, moving());
	draw_dynamic_content(score, 530, 30);
	draw_dynamic_content(punkte, 530, 60);
	AddUpdateRects(0, 0, hintergrund->w, hintergrund->h);
	Refresh();
	startTicks = (float)SDL_GetTicks();
	blinky_l.set_leader(1);	// Blinky als Referenz-Sprite fürs Neuzeichnen festgelegt
	// zuerst mal alle anhalten 
	stop_all(true, &pacman_l, &blinky_l, &pinky_l, &inky_l, &clyde_l); 

	// game loop
	while(loop) {	
		if(start_offset == -1)	
			loop = eventloop(hintergrund, &pacman_l, &blinky_l, &pinky_l, 
			&inky_l, &clyde_l, score);
		
		if(wechsel_counter > 50) {
			// ghost animations
			refresh_ghosts = 1;
			if(ghost_change)
				ghost_change = 0;
			else
				ghost_change = 1;
			blinky_l.animation(ghost_change);
			pinky_l.animation(ghost_change);
			inky_l.animation(ghost_change);
			clyde_l.animation(ghost_change);
			
			//
			if(pacman_l.is_dying) {
				if(pacman_l.is_dying > 1)
					pacman_l.is_dying--;
				else {
					//pacman = ar_pacman_die[die_counter];
					pacman_l.die_pic(die_counter);
					die_counter++;
					if(die_counter == 13) {
						pacman_l.is_dying = 0;
						reset(&pacman_l, &blinky_l, &pinky_l, &inky_l, &clyde_l, blinky_l.ghost);
						stop_all(true, &pacman_l, &blinky_l, &pinky_l, &inky_l, &clyde_l);
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
			
			// Anfangsoffset 
			if(start_offset > 0)
				start_offset--;
			else if (start_offset == 0) {
				stop_all(false, &pacman_l, &blinky_l, &pinky_l, &inky_l, &clyde_l);
				start_offset = -1;
		 	}
				
			wechsel_counter = 0;
		}
		else
			refresh_ghosts = 0;
		wechsel_counter = wechsel_counter + ms;
		
		draw_hintergrund(hintergrund);
		check_pillen(&pacman_l, &int_punktestand);
		compute_score(punkte, char_punktestand, int_punktestand, font, &textgelb); 
		draw_static_content(score, 530, 30);
		draw_pillen(pille, superpille[pille_counter]);
	
		if(pacman_l.wechsel()) {
			if(pacman_l.get_richtung() == 0)
		  		pacman_l.left_pic(ct_pm); 
		  	if(pacman_l.get_richtung() == 1) 
		   		pacman_l.up_pic(ct_pm); 
		  	if(pacman_l.get_richtung() == 2) 
		    	pacman_l.right_pic(ct_pm); 
		  	if(pacman_l.get_richtung() == 3) 
	    		pacman_l.down_pic(ct_pm); 
		  	ct_pm++;
		}
		if(ct_pm > 3)
			ct_pm = 0;
			
	  	// wenn Pacman steht, dann bitte auf "normal" stellen
		if(pacman_l.is_pacman_stopped() && !pacman_l.is_dying) {
			ct_pm = 0;
			if(pacman_l.get_richtung() == 0)
		  		pacman_l.left_pic(0); 
		  	if(pacman_l.get_richtung() == 1) 
		   		pacman_l.up_pic(0); 
		 	if(pacman_l.get_richtung() == 2) 
		    	pacman_l.right_pic(0); 
		  	if(pacman_l.get_richtung() == 3) 
	    		pacman_l.down_pic(0); 
			pacman_l.parking();
		}		
		
		blinky_l.draw(screen, moving());
		pinky_l.draw(screen, moving());
		inky_l.draw(screen, moving());
		clyde_l.draw(screen, moving());
		pacman_l.draw (screen);
		draw_blocks();
		Refresh();
		if(check_collision(pacman_l.pacman, blinky_l.ghost, pacman_l, ghost_array) && !pacman_l.is_dying) {
			stop_all(true, &pacman_l, &blinky_l, &pinky_l, &inky_l, &clyde_l);
			pacman_l.is_dying = 10;  
		}
				
		// für die korrekte Geschwindigkeit
		lastTickstemp = startTicks;
		startTicks = (float)SDL_GetTicks();
		lastTickstemp = startTicks - lastTickstemp;
		if(lastTickstemp <= WAIT_IN_MS) {
		  	SDL_Delay((int)(WAIT_IN_MS - lastTickstemp));
			ms = (float)(lastTickstemp/WAIT_IN_MS);
		}
		else 
			ms = (float)(lastTickstemp/WAIT_IN_MS);
		
		// und alle bewegen
		move_pacman(pacman_l.pacman, &pacman_l, ms);
		move_ghosts(blinky_l.ghost, &blinky_l, &pacman_l, ms);
		move_ghosts(pinky_l.ghost, &pinky_l, &pacman_l, ms);
		move_ghosts(inky_l.ghost, &inky_l, &pacman_l, ms);
		move_ghosts(clyde_l.ghost, &clyde_l, &pacman_l, ms);
		
	}
	SDL_FreeSurface(pille);
	SDL_FreeSurface(pacman_l.pacman);
	SDL_FreeSurface(hintergrund);
	
	return EXIT_SUCCESS;
}
