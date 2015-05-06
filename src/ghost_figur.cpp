#include "ghost_figur.h"
#include <stdlib.h>

Ghost::Ghost(int init_x, int init_y, int init_intelligence, 
             int init_richtung, int init_up_down, int ghost_ident,
             Screen *screen, Labyrinth *labyrinth):
	Figur(init_x, init_y, GHOSTS_V, screen, labyrinth),
	its_leader(0),
	initial_intelligence(init_intelligence),
	initial_richtung(init_richtung),
	initial_up_down(init_up_down),
	ghost_ident(ghost_ident)
{
	richtung = init_richtung;
	intelligence = init_intelligence;
	up_down = init_up_down;
	this->set_hunter(GHOST);

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
	escape_1 = LoadSurface("/usr/local/share/pacman/gfx/escaping_ghost_1.png", 255);
	escape_2 = LoadSurface("/usr/local/share/pacman/gfx/escaping_ghost_2.png", 255);
	escape_white_1 = LoadSurface("/usr/local/share/pacman/gfx/escaping_ghost_white_1.png", 0);
	escape_white_2 = LoadSurface("/usr/local/share/pacman/gfx/escaping_ghost_white_2.png", 0);
	ar_ghost[0] = ghost_1;
	ar_ghost[1] = ghost_2;
	ar_ghost[2] = escape_white_1;
	ar_ghost[3] = escape_white_2;
	num_animation_frames = 2;
	idx_animation = 1;
	this->ghost_sf = ar_ghost[idx_animation];
}

Ghost::~Ghost() {
	SDL_FreeSurface(ghost_1);
	SDL_FreeSurface(ghost_2);
	SDL_FreeSurface(augen_0);
	SDL_FreeSurface(augen_1);
	SDL_FreeSurface(augen_2);
	SDL_FreeSurface(augen_3);
	SDL_FreeSurface(escape_1);
	SDL_FreeSurface(escape_2);
	SDL_FreeSurface(escape_white_1);
	SDL_FreeSurface(escape_white_2);
}

void Ghost::draw() {
	if (this->get_hunter() != NONE)
		this->screen->draw(this->ghost_sf, this->x, this->y);
	if (this->get_hunter() != PACMAN) {
		switch(this->get_richtung()) {
			case 0:
				this->screen->draw(augen_0, this->x, this->y);
				break;
			case 1: 
				this->screen->draw(augen_1, this->x, this->y);
				break;
			case 2:
				this->screen->draw(augen_2, this->x, this->y);
				break;
			case 3: 
				this->screen->draw(augen_3, this->x, this->y);
				break;
			default:
				this->screen->draw(augen_0, this->x, this->y);
				break;
		}
	}
}

void Ghost::animation() {
	idx_animation = (idx_animation + 1) % num_animation_frames;
	this->ghost_sf = ar_ghost[idx_animation];
}

void Ghost::set_leader(int leader) {
	its_leader = leader;
}

void Ghost::set_leader() {
	int i;
	for(i = 0;i < 4; i++) {
		if(ghost_array[i]->getGhostIdent() == this->getGhostIdent())
			ghost_array[i]->set_leader(1);
		else
			ghost_array[i]->set_leader(0);
	}
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

void Ghost::move(int moving, Figur *pacman, float(ms)) {
	if(moving)
			this->AddUpdateRects_ghost();
	this->move_on_rails(pacman, ms, this->labyrinth->number_rails(), labyrinth->array_rails);
}

int Ghost::direction_to_point(int target_x, int target_y) {
	int dx = abs(target_x - this->x);
	int dy = abs(target_y - this->y);
	if (dx > dy) {
		return (target_x - this->x > 0) ? 2 /*to the right*/ : 0 /*to the left*/;
	} else {
		return (target_y - this->y > 0) ? 3 /*down*/ : 1 /*up*/;
	}
}

int Ghost::alternative_direction_to_point(int target_x, int target_y) {
	int dx = abs(target_x - this->x);
	int dy = abs(target_y - this->y);
	if (dx > dy) {
		// actually, here, a horizontal direction would be the best one, but what if it's not available?
		return (target_y - this->y > 0) ? 3 /*down*/ : 1 /*up*/;
	} else {
		// actually vertical, alternative will be horizontal
		return (target_x - this->x > 0) ? 2 /*to the right*/ : 0 /*to the left*/;
	}
}

int Ghost::get_intelligence() const {
	return intelligence;
}

int Ghost::choose_direction(int * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence) {
	int idx_dir_pacman = -1;
	int zufallswert;
	int idx_direction;
	int castle_direction;
	int castle_x = 310, castle_y = 190;

	if(sammel_counter == 1)
		return sammel_richtung[0];

	if (this->get_hunter() == NONE) {
		// try to return to the ghost castle
		castle_direction = this->direction_to_point(castle_x, castle_y);
		for (int i = 0; i < sammel_counter; ++i) {
			if (sammel_richtung[i] == castle_direction)
				return castle_direction;
		}
		// try an alternative direction
		castle_direction = this->alternative_direction_to_point(castle_x, castle_y);
		for (int i = 0; i < sammel_counter; ++i) {
			if (sammel_richtung[i] == castle_direction)
				return castle_direction;
		}
		// if neither the exact nor the alternative direction can be taken, then choose randomly
		zufallswert = (rand() % 100 + 1);
		return sammel_richtung[zufallswert%sammel_counter];
	}

	for(int i = 0; i < sammel_counter; ++i) {
		if(sammel_richtung[i] == richtung_pacman){
			idx_dir_pacman = i;
		}
	}
	zufallswert = (rand() % 100 + 1);
	if (this->get_hunter() == PACMAN) {
		if (idx_dir_pacman == -1 || zufallswert > intelligence) {
			return sammel_richtung[zufallswert%sammel_counter];
		} else {
			idx_direction = zufallswert % (sammel_counter-1);
			if (idx_direction == idx_dir_pacman)
				++idx_direction;
			return sammel_richtung[idx_direction];
		}
	} else {
		if(idx_dir_pacman != -1) {
			if(zufallswert <= intelligence) 
				return richtung_pacman;
		}
		return sammel_richtung[zufallswert%sammel_counter];
	}
}

void Ghost::move_on_rails(Figur *pacman, float ms, int anz_schienen, Rail **ar_s) {
	int i;
	int richtung_ghost = this->get_richtung();
	int richtung_pacman = this->direction_to_point(pacman->x, pacman->y);
	int sammel_richtung[3];
	int sammel_counter = 0;
	float max_links = 999;							// maximum to the left (otherwise, the ghost will leave the rails)
	float max_oben = 999;							// maximum up
	float max_rechts = 999;							// maximum to the right
	float max_unten = 999;							// maximum down
	int bridge;

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

		// for returning eyes-only ghosts, open the door to the castle
		bridge = (i==45 && this->get_hunter()==NONE) ? 1 : 0;

		// to the left
		if ((richtung_ghost == 0) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
			sammel_richtung[sammel_counter] = 1;
			sammel_counter++;	
			max_oben = this->cur_y - (float)ar_s[i]->y1;		
		}
		if ((richtung_ghost == 0) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1 - bridge)) {
			sammel_richtung[sammel_counter] = 3;
			sammel_counter++;
			max_unten = (float)ar_s[i]->y2 - this->cur_y;
		}
	  	if ((richtung_ghost == 0) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {		
			sammel_richtung[sammel_counter] = 0;
			sammel_counter++;
			max_links = this->cur_x - (float)ar_s[i]->x1;
		}
		if(sammel_counter == 3)
			break;
	
		// up
		if ((richtung_ghost == 1) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
			if(!this->up_down) {
				sammel_richtung[sammel_counter] = 0;
				sammel_counter++;
				max_links = this->cur_x - (float)ar_s[i]->x1;
			}
		}
		if ((richtung_ghost == 1) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
			if(!this->up_down) {
				sammel_richtung[sammel_counter] = 2;
				sammel_counter++;
				max_rechts = (float)ar_s[i]->x2 - this->cur_x;
			}
		}
		if ((richtung_ghost == 1) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
			sammel_richtung[sammel_counter] = 1;
			sammel_counter++;
			max_oben = this->cur_y - (float)ar_s[i]->y1;
		}
	
		if(sammel_counter == 3)
			break;
	
		// to the right
		if ((richtung_ghost == 2) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
			sammel_richtung[sammel_counter] = 1;
			sammel_counter++;
			max_oben = this->cur_y - (float)ar_s[i]->y1;
		}
		if ((richtung_ghost == 2) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1 - bridge)) {
			sammel_richtung[sammel_counter] = 3;
			sammel_counter++;
			max_unten = (float)ar_s[i]->y2 - this->cur_y;
		}
		if ((richtung_ghost == 2) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
			sammel_richtung[sammel_counter] = 2;
			sammel_counter++;
			max_rechts = (float)ar_s[i]->x2 - this->cur_x;
		}
		
		if(sammel_counter == 3)
			break;
	
		// down
		if ((richtung_ghost == 3) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
			if(!this->up_down) {
				sammel_richtung[sammel_counter] = 0;
				sammel_counter++;
				max_links = this->cur_x - (float)ar_s[i]->x1;
			}
		}
		if ((richtung_ghost == 3) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
			if(!this->up_down) {
				sammel_richtung[sammel_counter] = 2;
				sammel_counter++;
				max_rechts = (float)ar_s[i]->x2 - this->cur_x;
			}
		}
		if ((richtung_ghost == 3) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
			sammel_richtung[sammel_counter] = 3;
			sammel_counter++;
			max_unten = (float)ar_s[i]->y2 - this->cur_y;
		}
		if ((richtung_ghost == 3) && (i == 45) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y2)) {
			// special case: at the bottom of the castle's vertical center rail, the ghost must invert it's direction (i.e. go up again)
			sammel_richtung[0] = 1;
			sammel_counter = 1;
			max_oben = this->cur_y - (float)ar_s[i]->y1;
			// also make an eyes-only ghost normal again
			if (this->get_hunter() == NONE)
				this->set_hunter(GHOST);
			break;  // no more directions allowed
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
	cur_x = (float)initial_x;
	cur_y = (float)initial_y;
	richtung = initial_richtung;
	intelligence = initial_intelligence;
	up_down = initial_up_down;
	this->set_hunter(GHOST);
}

void Ghost::AddUpdateRects_ghost() {
	screen->AddUpdateRects(less(x,last_x), less(y,last_y), ghost_sf->w + abs(x-last_x), ghost_sf->h + abs(y-last_y));
}

SDL_Surface* Ghost::get_Surface() const {
	return ghost_sf;
}

Figur::Hunter Ghost::get_hunter() const {
	return hunter;
}

void Ghost::set_hunter(Hunter hunter) {
	if(hunter == PACMAN) {
		this->set_speed(0.1f);
		ar_ghost[0] = escape_1;
		ar_ghost[1] = escape_2;
	} else {
		this->set_speed(0.18f);
		ar_ghost[0] = ghost_1;
		ar_ghost[1] = ghost_2;
	}
	num_animation_frames = 2;
	this->hunter = hunter;
}

int Ghost::touched() {
	if(this->get_hunter() == PACMAN) {
		// ghost has been eaten by pacman
		this->hunter = NONE;
		this->set_speed(0.18f);
		this->set_leader();
	}
	if(this->get_hunter() == NONE)
		return 0;  // no problem for pacman
	return 1;
}

void Ghost::blink() {
	if(this->get_hunter() == PACMAN) {
		num_animation_frames = 4;
	}
}

void Ghost::setGhostArray(Ghost **ghost_array) {
	this->ghost_array = ghost_array;
}

Ghost::Ghosts Ghost::getGhostIdent() const {
	return (Ghosts)ghost_ident;
}
