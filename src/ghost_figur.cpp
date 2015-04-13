#include "ghost_figur.h"
#include <stdlib.h>

Ghost::Ghost(int init_x, int init_y, float init_v, int init_intelligence, 
             int init_richtung, int init_up_down, unsigned short int ghost_ident):
	Figur(init_x, init_y, init_v),
	its_leader(0),
	initial_intelligence(init_intelligence),
	initial_richtung(init_richtung),
	initial_up_down(init_up_down) {
	richtung = init_richtung;
	intelligence = init_intelligence;
	up_down = init_up_down;

	// Surfaces
	if(ghost_ident == BLINKY) {
		ghost_1 = LoadSurface("/usr/local/share/pacman/gfx/blinky_1.png", 255);
        ghost_2 = LoadSurface("/usr/local/share/pacman/gfx/blinky_2.png", 255);
	}
	else if(ghost_ident == PINKY) {
		ghost_1 = LoadSurface("/usr/local/share/pacman/gfx/pinky_1.png", 255);
        ghost_2 = LoadSurface("/usr/local/share/pacman/gfx/pinky_2.png", 255);
	}
	else if(ghost_ident == INKY) {
		ghost_1 = LoadSurface("/usr/local/share/pacman/gfx/inky_1.png", 255);
        ghost_2 = LoadSurface("/usr/local/share/pacman/gfx/inky_2.png", 255);
	}
	else if(ghost_ident == CLYDE) {
		ghost_1 = LoadSurface("/usr/local/share/pacman/gfx/clyde_1.png", 255);
        ghost_2 = LoadSurface("/usr/local/share/pacman/gfx/clyde_2.png", 255);
	}
	augen_0 = LoadSurface("/usr/local/share/pacman/gfx/augen_0.png", 0);
    augen_1 = LoadSurface("/usr/local/share/pacman/gfx/augen_1.png", 0);
    augen_2 = LoadSurface("/usr/local/share/pacman/gfx/augen_2.png", 0);
    augen_3 = LoadSurface("/usr/local/share/pacman/gfx/augen_3.png", 0);
	ar_ghost[0] = ghost_1;
	ar_ghost[1] = ghost_2;
	this->ghost_sf = ar_ghost[1];
}

Ghost::~Ghost() {
	SDL_FreeSurface(ghost_1);
	SDL_FreeSurface(ghost_2);
	SDL_FreeSurface(augen_0);
	SDL_FreeSurface(augen_1);
	SDL_FreeSurface(augen_2);
	SDL_FreeSurface(augen_3);
}

void Ghost::draw(SDL_Surface *screen, int moving) {
	SDL_Rect dest;
	dest.x = this->x;
	dest.y = this->y;
	SDL_BlitSurface(this->ghost_sf, NULL, screen, &dest);
	if(moving) {
		switch(this->get_richtung()) {
			case 0:
				SDL_BlitSurface(augen_0, NULL, screen, &dest);
				break;
			case 1: 
				SDL_BlitSurface(augen_1, NULL, screen, &dest);
				break;
			case 2:
				SDL_BlitSurface(augen_2, NULL, screen, &dest);
				break;
			case 3: 
				SDL_BlitSurface(augen_3, NULL, screen, &dest);
				break;
			default:
				SDL_BlitSurface(augen_0, NULL, screen, &dest);
				break;
		}
	}
}

void Ghost::animation(int cnt_pic) {
	this->ghost_sf = ar_ghost[cnt_pic];
}

void Ghost::set_leader(int leader) {
	its_leader = leader;
}

void Ghost::move(float ms, int direction, float max_links, float max_oben, float max_rechts, float max_unten) {
	if(direction == 0)
		move_left(ms, max_links);
	if(direction == 1)
		move_up(ms, max_oben);
	if(direction == 2)
		move_right(ms, max_rechts);
	if(direction == 3)
		move_down(ms, max_unten);	
	if(its_leader) {
		if(this->was_moving())
			was_moving_leader = 1;
		else
			was_moving_leader = 0;
	}	
}

int Ghost::relative_pos_pacman(Pacman *pacman) {
	int dx, dy;
	dx = abs(pacman->x - this->x);
	dy = abs(pacman->y - this->y);
	if(dx > dy) {
		if((pacman->x - this->x) > 0) 
			return 2;	// to the right
		else
			return 0;	// to the left
	}
	else {
		if((pacman->y - this->y) > 0) 
			return 3;	// down
		else
			return 1;	// up
	}
}

int Ghost::get_intelligence() const {
	return intelligence;
}

int Ghost::choose_direction(int * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence) {
	int i;
	int ist_richtung_pacman = -1;
	int zufallswert;
	
	srand(time(0));
	for(i = 0;i < sammel_counter;i++) {
		if(*(sammel_richtung + i) == richtung_pacman){
			ist_richtung_pacman = i;
		}
	}
	if(sammel_counter == 1)
		return *sammel_richtung;
	//srand(time(0));
	zufallswert = (rand() % 100 + 1);
	if(ist_richtung_pacman != -1) {
		if(zufallswert <= intelligence) 
			return richtung_pacman;
	}
	if(sammel_counter == 2) {
		if(zufallswert <= 50)
			return *sammel_richtung;
		else
			return *(sammel_richtung + 1);
	}
	if(sammel_counter == 3) {
	// first, define the static member variable
		if(zufallswert <= 33)
			return *sammel_richtung;
		if((zufallswert > 33) && (zufallswert <= 66))
			return *(sammel_richtung + 1);
		if((zufallswert > 66) && (zufallswert <= 100))
			return *(sammel_richtung + 2);
	}
	return *sammel_richtung; // suppress compiler warning
}


void Ghost::move_on_rails(Pacman *pacman, float ms, int anz_schienen, Schiene **ar_s) {
	int i;
	int richtung_ghost = this->get_richtung();
	int richtung_pacman = this->relative_pos_pacman(pacman);
	int sammel_richtung[3];
	int sammel_counter = 0;
	float max_links = 999;							// maximum to the left (otherwise, the ghost will leave the rails)
	float max_oben = 999;							// maximum up
	float max_rechts = 999;							// maximum to the right
	float max_unten = 999;							// maximum down
	
	
		
		for(i = 0; i <= anz_schienen - 1; i++) {	
			// first, check the tunnel
			if((richtung_ghost != 2) && (this->x <= 100) && (this->y == 215)) {
		 		this->x = 515;
		 		this->cur_x = 515;
		 		richtung_ghost = 0;
		 		break;
			}
			if((richtung_ghost != 0) && (this->x >= 515) && (this->y == 215)) {
		 		this->x = 100;
		 		this->cur_x = 100;
		 		richtung_ghost = 2;
		 		break;
			}

			// to the left
			if ((richtung_ghost == 0) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
				sammel_richtung[sammel_counter] = 1;
				sammel_counter++;	
				max_oben = this->cur_y - ar_s[i]->y1;		
			}
			if ((richtung_ghost == 0) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
				sammel_richtung[sammel_counter] = 3;
				sammel_counter++;
				max_unten = ar_s[i]->y2 - this->cur_y;
			}
		  	if ((richtung_ghost == 0) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {		
				sammel_richtung[sammel_counter] = 0;
				sammel_counter++;
				max_links = this->cur_x - ar_s[i]->x1;
			}
			if(sammel_counter == 3)
				break;
		
			// up
			if ((richtung_ghost == 1) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
				if(!this->up_down) {
					sammel_richtung[sammel_counter] = 0;
					sammel_counter++;
					max_links = this->cur_x - ar_s[i]->x1;
				}
			}
			if ((richtung_ghost == 1) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
				if(!this->up_down) {
					sammel_richtung[sammel_counter] = 2;
					sammel_counter++;
					max_rechts = ar_s[i]->x2 - this->cur_x;
				}
			}
			if ((richtung_ghost == 1) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
				sammel_richtung[sammel_counter] = 1;
				sammel_counter++;
				max_oben = this->cur_y - ar_s[i]->y1;
			}
		
			if(sammel_counter == 3)
				break;
		
			// to the right
			if ((richtung_ghost == 2) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
				sammel_richtung[sammel_counter] = 1;
				sammel_counter++;
				max_oben = this->cur_y - ar_s[i]->y1;
			}
			if ((richtung_ghost == 2) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
				sammel_richtung[sammel_counter] = 3;
				sammel_counter++;
				max_unten = ar_s[i]->y2 - this->cur_y;
			}
			if ((richtung_ghost == 2) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
				sammel_richtung[sammel_counter] = 2;
				sammel_counter++;
				max_rechts = ar_s[i]->x2 - this->cur_x;
			}
			
			if(sammel_counter == 3)
				break;
		
			// down
			if ((richtung_ghost == 3) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
				if(!this->up_down) {
					sammel_richtung[sammel_counter] = 0;
					sammel_counter++;
					max_links = this->cur_x - ar_s[i]->x1;
				}
			}
			if ((richtung_ghost == 3) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
				if(!this->up_down) {
					sammel_richtung[sammel_counter] = 2;
					sammel_counter++;
					max_rechts = ar_s[i]->x2 - this->cur_x;
				}
			}
			if ((richtung_ghost == 3) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
				sammel_richtung[sammel_counter] = 3;
				sammel_counter++;
				max_unten = ar_s[i]->y2 - this->cur_y;
			}
		
			if(sammel_counter == 3)
				break;
		}
	
	
		if((richtung_ghost == 1) && (sammel_counter == 0) && (this->up_down)) {
			sammel_richtung[sammel_counter] = 3;
			sammel_counter++;
			this->up_down--;
		}
	
		if((richtung_ghost == 3) && (sammel_counter == 0) && (this->up_down)) {
			sammel_richtung[sammel_counter] = 1;
			sammel_counter++;
			this->up_down--;
		}
	
		if(sammel_counter != 0){
				if(this->was_moving())
					richtung_ghost = choose_direction(sammel_richtung, richtung_pacman, sammel_counter, this->get_intelligence());
		}

	this->move(ms,richtung_ghost, max_links, max_oben, max_rechts, max_unten);
}

void Ghost::reset() {
	x = initial_x;
	y = initial_y;
	dx = initial_v;
	dy = initial_v;
	last_x = initial_x;
	last_y = initial_y;
	cur_x = initial_x;
	cur_y = initial_y;
	richtung = initial_richtung;
	intelligence = initial_intelligence;
	up_down = initial_up_down;
}

