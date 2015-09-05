#include "ghost_figur.h"
#include <stdlib.h>

Ghost **Ghost::allGhosts = new Ghost*[Constants::TOTAL_NUM_GHOSTS];
int Ghost::numGhosts = 0;
int Ghost::was_moving_leader = 1;

Ghost::Ghost(int init_x, int init_y, int init_intelligence,
             Direction init_direction, int init_up_down, int ghost_ident,
             Pacman *pacman):
	Figur(init_x, init_y, Constants::GHOSTS_V_NORMAL),
	its_leader(0),
	initial_intelligence(init_intelligence),
	initial_direction(init_direction),
	initial_up_down(init_up_down),
	ghost_ident(ghost_ident),
	idxCurrentRail(-1)  // should first be determined
{
	if (numGhosts < Constants::TOTAL_NUM_GHOSTS) {
		allGhosts[numGhosts] = this;
		++numGhosts;
	}
	direction = init_direction;
	intelligence = init_intelligence;
	up_down = init_up_down;
	this->set_hunter(GHOST);
	this->pacman = pacman;
	char filePath[256];
	// Surfaces
	if(ghost_ident == BLINKY) {
		getFilePath(filePath, "gfx/blinky_1.png");
		ghost_1 = Screen::getInstance()->LoadSurface(filePath, 255);
		getFilePath(filePath, "gfx/blinky_2.png");
		ghost_2 = Screen::getInstance()->LoadSurface(filePath, 255);
		idxCurrentRail = 16;
	}
	else if(ghost_ident == PINKY) {
		getFilePath(filePath, "gfx/pinky_1.png");
		ghost_1 = Screen::getInstance()->LoadSurface(filePath, 255);
		getFilePath(filePath, "gfx/pinky_2.png");
		ghost_2 = Screen::getInstance()->LoadSurface(filePath, 255);
		idxCurrentRail = 89;
	}
	else if(ghost_ident == INKY) {
		getFilePath(filePath, "gfx/inky_1.png");
		ghost_1 = Screen::getInstance()->LoadSurface(filePath, 255);
		getFilePath(filePath, "gfx/inky_2.png");
		ghost_2 = Screen::getInstance()->LoadSurface(filePath, 255);
		idxCurrentRail = 88;
	}
	else if(ghost_ident == CLYDE) {
		getFilePath(filePath, "gfx/clyde_1.png");
		ghost_1 = Screen::getInstance()->LoadSurface(filePath, 255);
		getFilePath(filePath, "gfx/clyde_2.png");
		ghost_2 = Screen::getInstance()->LoadSurface(filePath, 255);
		idxCurrentRail = 90;
	}
	getFilePath(filePath, "gfx/augen_0.png");
	augen_0 = Screen::getInstance()->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/augen_1.png");
	augen_1 = Screen::getInstance()->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/augen_2.png");
	augen_2 = Screen::getInstance()->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/augen_3.png");
	augen_3 = Screen::getInstance()->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/escaping_ghost_1.png");
	escape_1 = Screen::getInstance()->LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/escaping_ghost_2.png");
	escape_2 = Screen::getInstance()->LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/escaping_ghost_white_1.png");
	escape_white_1 = Screen::getInstance()->LoadSurface(filePath, 0);
	getFilePath(filePath, "gfx/escaping_ghost_white_2.png");
	escape_white_2 = Screen::getInstance()->LoadSurface(filePath, 0);
	ar_ghost[0] = ghost_1;
	ar_ghost[1] = ghost_2;
	ar_ghost[2] = escape_white_1;
	ar_ghost[3] = escape_white_2;
	num_animation_frames = 2;
	idx_animation = 1;
	this->ghost_sf = ar_ghost[idx_animation];
}

Ghost::~Ghost() {
	for (int i = 0; i < numGhosts; ++i) {
		if (allGhosts[i] == this) {
			for (int j = i; j < numGhosts-1; ++j) {
				allGhosts[j] = allGhosts[j+1];
			}
			allGhosts[numGhosts-1] = NULL;
			--numGhosts;
			break;
		}
	}
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
	if (this->visible) {
		if (this->get_hunter() != NONE)
			Screen::getInstance()->draw(this->ghost_sf, this->x, this->y);
		if (this->get_hunter() != PACMAN) {
			switch(this->get_direction()) {
				case LEFT:
					Screen::getInstance()->draw(augen_0, this->x, this->y);
					break;
				case UP:
					Screen::getInstance()->draw(augen_1, this->x, this->y);
					break;
				case RIGHT:
					Screen::getInstance()->draw(augen_2, this->x, this->y);
					break;
				case DOWN:
					Screen::getInstance()->draw(augen_3, this->x, this->y);
					break;
				default:
					Screen::getInstance()->draw(augen_0, this->x, this->y);
					break;
			}
		}
	}
}

void Ghost::animation() {
	idx_animation = (idx_animation + 1) % num_animation_frames;
	this->ghost_sf = ar_ghost[idx_animation];
}

void Ghost::set_leader(bool leader) {
	its_leader = leader;
}

void Ghost::set_leader() {
	int i;
	for(i = 0;i < 4; i++) {
		if(allGhosts[i]->getGhostIdent() == this->getGhostIdent())
			allGhosts[i]->set_leader(true);
		else
			allGhosts[i]->set_leader(false);
	}
}

void Ghost::move_dir(int ms, int direction, int max_links, int max_oben, int max_rechts, int max_unten) {
	if(direction == LEFT)
		move_left(ms, max_links);
	if(direction == UP)
		move_up(ms, max_oben);
	if(direction == RIGHT)
		move_right(ms, max_rechts);
	if(direction == DOWN)
		move_down(ms, max_unten);
	if(its_leader) {
		if(this->was_moving())
			was_moving_leader = 1;
		else
			was_moving_leader = 0;
	}
}

void Ghost::move(int ms) {
	this->move_on_rails(ms, Labyrinth::getInstance()->array_rails);
	if (last_x != x || last_y != y)
		this->addUpdateRect();
}

int Ghost::direction_to_point(int target_x, int target_y) {
	int dx = abs(target_x - this->x);
	int dy = abs(target_y - this->y);
	if (dx > dy) {
		return (target_x - this->x > 0) ? RIGHT : LEFT;
	} else {
		return (target_y - this->y > 0) ? DOWN : UP;
	}
}

int Ghost::alternative_direction_to_point(int target_x, int target_y) {
	int dx = abs(target_x - this->x);
	int dy = abs(target_y - this->y);
	if (dx > dy) {
		// actually, here, a horizontal direction would be the best one, but what if it's not available?
		return (target_y - this->y > 0) ? DOWN : UP;
	} else {
		// actually vertical, alternative will be horizontal
		return (target_x - this->x > 0) ? RIGHT : LEFT;
	}
}

int Ghost::get_intelligence() const {
	return intelligence;
}

int Ghost::choose_direction(Direction * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence) {
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

void Ghost::move_on_rails(int ms, Rail **ar_s) {
	bool moved = false;
	Direction old_dir = get_direction();
	Direction sammel_richtung[3];
	int sammel_counter = 0;
	int idxLeft = -1, idxRight = -1, idxUp = -1, idxDown = -1;
	if (idxCurrentRail >= 0) {
		// first, try to keep moving on the current rail
		if (ar_s[idxCurrentRail]->y1 == ar_s[idxCurrentRail]->y2) {
			// horizontal rail
			if (old_dir == LEFT && this->x > ar_s[idxCurrentRail]->x1 - ((idxCurrentRail==87)?1:0)) {
				this->move_left(ms, ar_s[idxCurrentRail]->x1 - ((idxCurrentRail==87)?1:0));
				moved = true;
			} else if (old_dir == RIGHT && this->x < ar_s[idxCurrentRail]->x2 + ((idxCurrentRail==86)?1:0)) {
				this->move_right(ms, ar_s[idxCurrentRail]->x2 + ((idxCurrentRail==86)?1:0));
				moved = true;
			}
		} else {
			// vertical rail
			if (old_dir == UP && this->y > ar_s[idxCurrentRail]->y1 - ((idxCurrentRail==89)?1:0)) {
				this->move_up(ms, ar_s[idxCurrentRail]->y1 - ((idxCurrentRail==89)?1:0));
				moved = true;
			} else if (old_dir == DOWN && this->y < ar_s[idxCurrentRail]->y2) {
				this->move_down(ms, ar_s[idxCurrentRail]->y2);
				moved = true;
			}
		}
	}
	if (!moved) {
		// check the tunnel
		if ((old_dir != RIGHT) && (this->x <= 100) && (this->y == 215)) {
	 		this->x = 515;
	 		this->cur_x = this->x << 10;
	 		old_dir = LEFT;
	 		idxCurrentRail = 22;
	 		moved = true;
		} else if ((old_dir != LEFT) && (this->x >= 515) && (this->y == 215)) {
	 		this->x = 100;
	 		this->cur_x = this->x << 10;
	 		old_dir = RIGHT;
	 		idxCurrentRail = 19;
	 		moved = true;
		}
	}
	if (!moved) {
		if (idxCurrentRail==89 && old_dir==DOWN && this->y>=ar_s[89]->y2) {
			// special case: at the bottom of the castle's vertical center rail, the ghost must invert it's direction (i.e. go up again)
			sammel_richtung[0] = UP;
			sammel_counter = 1;
			// make an eyes-only ghost normal again
			if (get_hunter() == NONE) {
				set_hunter(GHOST);
				Sounds::getInstance()->eat_ghost_stop();
			}
		} else if (this->up_down) {
			sammel_richtung[sammel_counter] = ((old_dir==UP) ? DOWN : UP);
			++sammel_counter;
			this->up_down--;
		} else {
			// Find the rails that the ghost may take next
			Labyrinth::getInstance()->getRailsForPoint(this->x, this->y, &idxLeft, &idxRight, &idxUp, &idxDown);
			// eliminate the direction the ghost came from
			switch (old_dir) {
			case LEFT:
				idxRight = -1;
				break;
			case RIGHT:
				idxLeft = -1;
				break;
			case UP:
				idxDown = -1;
				break;
			case DOWN:
				idxUp = -1;
				break;
			}
			// for a returning eyes-only ghost, open the door to the castle
			if (get_hunter() == NONE && this->x == ar_s[89]->x1 && this->y == ar_s[89]->y1-1)
				idxDown = 89;
			// eliminate the directions where the rail end has already been reached
			if (old_dir == LEFT && idxLeft>=0 && this->x <= ar_s[idxLeft]->x1 && ar_s[idxLeft]->y1==ar_s[idxLeft]->y2)
				idxLeft = -1;
			if (old_dir == RIGHT && idxRight>=0 && this->x >= ar_s[idxRight]->x2 && ar_s[idxLeft]->y1==ar_s[idxLeft]->y2)
				idxRight = -1;
			if (old_dir == UP && idxUp>=0 && this->y <= ar_s[idxUp]->y1 && ar_s[idxLeft]->x1==ar_s[idxLeft]->x2)
				idxUp = -1;
			if (old_dir == DOWN && idxDown>=0 && this->y >= ar_s[idxDown]->y2 && ar_s[idxLeft]->x1==ar_s[idxLeft]->x2)
				idxDown = -1;
			// insert the directions to the array
			if (idxLeft >= 0) {
				sammel_richtung[sammel_counter] = LEFT;
				++sammel_counter;
			}
			if (idxRight >= 0) {
				sammel_richtung[sammel_counter] = RIGHT;
				++sammel_counter;
			}
			if (idxUp >= 0) {
				sammel_richtung[sammel_counter] = UP;
				++sammel_counter;
			}
			if (idxDown >= 0) {
				sammel_richtung[sammel_counter] = DOWN;
				++sammel_counter;
			}
		}
	}
	if (!moved && sammel_counter > 0) {
		int richtung_pacman = this->direction_to_point(pacman->x, pacman->y);
		int new_dir = choose_direction(sammel_richtung, richtung_pacman, sammel_counter, get_intelligence());
		if (new_dir==LEFT && idxLeft>=0)
			idxCurrentRail = idxLeft;
		if (new_dir==RIGHT && idxRight>=0)
			idxCurrentRail = idxRight;
		if (new_dir==UP && idxUp>=0)
			idxCurrentRail = idxUp;
		if (new_dir==DOWN && idxDown>=0)
			idxCurrentRail = idxDown;
		int maxLeft  = ((idxCurrentRail>=0) ? ar_s[idxCurrentRail]->x1 : 0);
		int maxRight = ((idxCurrentRail>=0) ? ar_s[idxCurrentRail]->x2 : 10000);
		int maxUp    = ((idxCurrentRail>=0) ? ar_s[idxCurrentRail]->y1 : 0);
		int maxDown  = ((idxCurrentRail>=0) ? ar_s[idxCurrentRail]->y2 : 10000);
		move_dir(ms, new_dir, maxLeft, maxUp, maxRight, maxDown);
	}
}

void Ghost::reset() {
	x = initial_x;
	y = initial_y;
	dx = initial_v;
	dy = initial_v;
	last_x = initial_x;
	last_y = initial_y;
	cur_x = initial_x << 10;
	cur_y = initial_y << 10;
	direction = initial_direction;
	intelligence = initial_intelligence;
	up_down = initial_up_down;
	this->set_hunter(GHOST);
	if (ghost_ident == BLINKY)
		idxCurrentRail = 16;
	if (ghost_ident == PINKY)
		idxCurrentRail = 89;
	if (ghost_ident == INKY)
		idxCurrentRail = 88;
	if (ghost_ident == CLYDE)
		idxCurrentRail = 90;
	visible = true;
}

void Ghost::addUpdateRect() {
	Screen::getInstance()->AddUpdateRects(least(x,last_x), least(y,last_y), ghost_sf->w + abs(x-last_x), ghost_sf->h + abs(y-last_y));
}

SDL_Surface* Ghost::get_Surface() const {
	return ghost_sf;
}

Figur::Hunter Ghost::get_hunter() const {
	return hunter;
}

void Ghost::set_hunter(Hunter hunter) {
	if(hunter == PACMAN) {
		this->set_speed(Constants::GHOSTS_V_SLOW);
		ar_ghost[0] = escape_1;
		ar_ghost[1] = escape_2;
	} else {
		this->set_speed(Constants::GHOSTS_V_NORMAL);
		ar_ghost[0] = ghost_1;
		ar_ghost[1] = ghost_2;
	}
	num_animation_frames = 2;
	this->hunter = hunter;
}

bool Ghost::touched() {
	if(this->get_hunter() == PACMAN) {
		// ghost has been eaten by pacman
		hunter = NONE;
		set_speed(Constants::GHOSTS_V_NORMAL);
		set_leader();
		setVisibility(0);
		pacman->setVisibility(0);
		Labyrinth::getInstance()->addBonusScore(this->x + (ghost_sf->w >> 1), this->y + (ghost_sf->h >> 1));
		Labyrinth::getInstance()->increaseBonusStage();
		Labyrinth::getInstance()->sleep(400);
		Labyrinth::getInstance()->playEatGhost();
	}
	if(get_hunter() == NONE)
		return false;  // no problem for pacman
	return true;
}

void Ghost::blink() {
	if(this->get_hunter() == PACMAN) {
		num_animation_frames = 4;
	}
}

Ghost::Ghosts Ghost::getGhostIdent() const {
	return (Ghosts)ghost_ident;
}
