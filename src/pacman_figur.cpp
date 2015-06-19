#include "pacman_figur.h"
#include "math.h"

Pacman::Pacman(int init_x, int init_y, Screen *screen, Labyrinth *labyrinth, int lives):
	Figur(init_x, init_y, PACMAN_V_FAST, screen, labyrinth),
	animation(0),
	cnt_animation(0),
	pacman_stopped(0),
	dying(0),
	die_counter(0),
	remainingLives(lives),
	idxCurrentRail(33)
{
    wechsel_rate = WECHSEL_RATE;
	wechsel_x = init_x;
	wechsel_y = init_y;
	direction = LEFT;
	direction_pre = LEFT;
	char filePath[256];
	getFilePath(filePath, "gfx/pacman.png");
	pacman_normal = Figur::LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_links_1.png");	
	pacman_links_1 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_links_2.png");	
    pacman_links_2 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_oben_1.png");	
    pacman_oben_1 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_oben_2.png");	
    pacman_oben_2 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_rechts_1.png");	
    pacman_rechts_1 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_rechts_2.png");	
    pacman_rechts_2 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_unten_1.png");	
    pacman_unten_1 = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_unten_2.png");	
    pacman_unten_2 = LoadSurface(filePath, 255);
	// initialize all pacman images
    ar_pacman_links[0] = pacman_links_1;
    ar_pacman_links[1] = pacman_links_2;
    ar_pacman_links[2] = pacman_links_1;
    ar_pacman_links[3] = pacman_normal;
        
    ar_pacman_oben[0] = pacman_oben_1;
    ar_pacman_oben[1] = pacman_oben_2;
    ar_pacman_oben[2] = pacman_oben_1;
    ar_pacman_oben[3] = pacman_normal;
        
    ar_pacman_rechts[0] = pacman_rechts_1;
    ar_pacman_rechts[1] = pacman_rechts_2;
    ar_pacman_rechts[2] = pacman_rechts_1;
    ar_pacman_rechts[3] = pacman_normal;
        
    ar_pacman_unten[0] = pacman_unten_1;
    ar_pacman_unten[1] = pacman_unten_2;
    ar_pacman_unten[2] = pacman_unten_1;
    ar_pacman_unten[3] = pacman_normal;

	getFilePath(filePath, "gfx/pacman_die_1.png");	
    ar_pacman_die[0] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_2.png");	
    ar_pacman_die[1] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_3.png");	
    ar_pacman_die[2] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_4.png");	
    ar_pacman_die[3] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_5.png");	
    ar_pacman_die[4] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_6.png");	
    ar_pacman_die[5] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_10.png");	
	ar_pacman_die[6] = LoadSurface(filePath, 255);
	ar_pacman_die[7] = LoadSurface(filePath, 255);
	ar_pacman_die[8] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_7.png");	
	ar_pacman_die[9] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_8.png");	
    ar_pacman_die[10] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_9.png");	
    ar_pacman_die[11] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_7.png");	
    ar_pacman_die[12] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_8.png");	
    ar_pacman_die[13] = LoadSurface(filePath, 255);
	getFilePath(filePath, "gfx/pacman_die_9.png");	
    ar_pacman_die[14] = LoadSurface(filePath, 255);
    ar_pacman_die[15] = LoadSurface(filePath, 255);
	
	this->pacman_sf = ar_pacman_links[0];
}

Pacman::~Pacman() {
	SDL_FreeSurface(pacman_normal);
	SDL_FreeSurface(pacman_links_1);
	SDL_FreeSurface(pacman_links_2);
	SDL_FreeSurface(pacman_oben_1);
	SDL_FreeSurface(pacman_oben_2);
	SDL_FreeSurface(pacman_rechts_1);
	SDL_FreeSurface(pacman_rechts_2);
	SDL_FreeSurface(pacman_unten_1);
	SDL_FreeSurface(pacman_unten_2);
	for(int i = 0; i < 13; i++)
		SDL_FreeSurface(ar_pacman_die[i]);
}

void Pacman::draw() {
	if (this->visible)
	    this->screen->draw(this->pacman_sf, this->x, this->y);
}

void Pacman::checkAnimationChange() {
	if ((wechsel_rate > 0) && ((abs(x-wechsel_x)>=wechsel_rate)||(abs(y-wechsel_y)>=wechsel_rate))) {
		wechsel_x = x;
		wechsel_y = y;
		animation = 1;
	}
}

void Pacman::move(int ms) {
	this->move_on_rails(ms, this->labyrinth->number_rails(), this->labyrinth->array_rails);		
	if (last_x != x || last_y != y)
		this->addUpdateRect();
}
void Pacman::move_left(int ms, int stop_at) {
	Figur::move_left(ms, stop_at);
	checkAnimationChange();
}

void Pacman::move_up(int ms, int stop_at) {
	Figur::move_up(ms, stop_at);
	checkAnimationChange();
}

void Pacman::move_right(int ms, int stop_at) {
	Figur::move_right(ms, stop_at);
	checkAnimationChange();
}

void Pacman::move_down(int ms, int stop_at) {
	Figur::move_down(ms, stop_at);
	checkAnimationChange();
}

void Pacman::left_pic(int cnt_pic) {
	this->pacman_sf = ar_pacman_links[cnt_pic];
}

void Pacman::up_pic(int cnt_pic) {
	this->pacman_sf = ar_pacman_oben[cnt_pic];
}

void Pacman::right_pic(int cnt_pic) {
	this->pacman_sf = ar_pacman_rechts[cnt_pic];
}

void Pacman::down_pic(int cnt_pic) {
	this->pacman_sf = ar_pacman_unten[cnt_pic];
}

void Pacman::die_pic(int cnt_pic) {
	this->pacman_sf = ar_pacman_die[cnt_pic];
}
void Pacman::animate() {
	if (animation) {
		animation = 0;
		switch(this->get_direction()) {
			case 0:
				this->left_pic(cnt_animation);
				break;
			case 1:
				this->up_pic(cnt_animation);
				break;
			case 2:
				this->right_pic(cnt_animation);
				break;
			case 3:
				this->down_pic(cnt_animation);
		}
	  	cnt_animation++;
	}
	if(cnt_animation > 3)
			cnt_animation = 0;
}

void Pacman::move_on_rails(int ms, int anz_schienen, Rail **ar_s) {
	bool moved = false;
	Direction old_dir, new_dir;
	new_dir = old_dir = get_direction();
	if (idxCurrentRail >= 0) {
		// first, try to keep moving on the current rail
		if (ar_s[idxCurrentRail]->y1 == ar_s[idxCurrentRail]->y2) {
			// horizontal rail
			if ((this->direction_pre==LEFT || (old_dir==LEFT && this->direction_pre!=RIGHT)) && this->x > ar_s[idxCurrentRail]->x1) {
				new_dir = LEFT;
				this->move_left(ms, ar_s[idxCurrentRail]->x1);
				moved = true;
			} else if ((this->direction_pre==RIGHT || (old_dir==RIGHT && this->direction_pre!=LEFT)) && this->x < ar_s[idxCurrentRail]->x2) {
				new_dir = RIGHT;
				this->move_right(ms, ar_s[idxCurrentRail]->x2);
				moved = true;
			}
		} else {
			// vertical rail
			if ((this->direction_pre==UP || (old_dir==UP && this->direction_pre!=DOWN)) && this->y > ar_s[idxCurrentRail]->y1) {
				new_dir = UP;
				this->move_up(ms, ar_s[idxCurrentRail]->y1);
				moved = true;
			} else if ((this->direction_pre==DOWN || (old_dir==DOWN && this->direction_pre!=UP)) && this->y < ar_s[idxCurrentRail]->y2) {
				new_dir = DOWN;
				this->move_down(ms, ar_s[idxCurrentRail]->y2);
				moved = true;
			}
		}
	} else {
		this->direction_pre = old_dir;
	}
	if (!moved) {
		// check the tunnel
		if((this->direction_pre != RIGHT) && (this->x <= 100) && (this->y == 215)) {
	 		this->x = 515;
	 		this->cur_x = this->x << 10;
	 		this->direction_pre = LEFT;
	 		idxCurrentRail = 22;
	 		moved = true;
		} else if((this->direction_pre != LEFT) && (this->x >= 515) && (this->y == 215)) {
	 		this->x = 100;
	 		this->cur_x = this->x << 10;
	 		this->direction_pre = RIGHT;
	 		idxCurrentRail = 19;
	 		moved = true;
		}
	}
	if (!moved) {
		// Find the rail that pacman is positioned on. Also, consider the direction that the player wants to move.
		int idxLeft, idxRight, idxUp, idxDown;
		this->labyrinth->getRailsForPoint(this->x, this->y, &idxLeft, &idxRight, &idxUp, &idxDown);
		if (idxLeft>=0 && this->x <= ar_s[idxLeft]->x1)
			idxLeft = -1;
		if (idxRight>=0 && ar_s[idxRight]->x2 <= this->x)
			idxRight = -1;
		if (idxUp>=0 && this->y <= ar_s[idxUp]->y1)
			idxUp = NULL;
		if (idxDown>=0 && ar_s[idxDown]->y2 <= this->y)
			idxDown = NULL;
		bool use_old_dir = false;
		if ((this->direction_pre==LEFT && idxLeft<0) || (this->direction_pre==RIGHT && idxRight<0) || (this->direction_pre==UP && idxUp<0) || (this->direction_pre==DOWN && idxDown<0))
			use_old_dir = true;
		new_dir = use_old_dir ? old_dir : this->direction_pre;
		switch (new_dir) {
		case LEFT:
			if (idxLeft >= 0) {
				move_left(ms, ar_s[idxLeft]->x1);
				idxCurrentRail = idxLeft;
				moved = true;
			}
			break;
		case RIGHT:
			if (idxRight >= 0) {
				move_right(ms, ar_s[idxRight]->x2);
				idxCurrentRail = idxRight;
				moved = true;
			}
			break;
		case UP:
			if (idxUp >= 0) {
				move_up(ms, ar_s[idxUp]->y1);
				idxCurrentRail = idxUp;
				moved = true;
			}
			break;
		case DOWN:
			if (idxDown >= 0) {
				move_down(ms, ar_s[idxDown]->y2);
				idxCurrentRail = idxDown;
				moved = true;
			}
			break;
		}
	}
	if (new_dir != old_dir)
		this->animation = 1;
	if (!moved) 
		this->pacman_stopped = 1;
}

int Pacman::is_pacman_stopped() {
	if(pacman_stopped) {
		pacman_stopped = 0;
		return 1;
	} else
		return 0;	
}

void Pacman::reset() {
	x = initial_x;
	y = initial_y;
	dx = initial_v;
	dy = initial_v;
	last_x = initial_x;
	last_y = initial_y;
	cur_x = initial_x << 10;
	cur_y = initial_y << 10;
	wechsel_x = initial_x;
	wechsel_y = initial_y;
	direction = LEFT;
	direction_pre = LEFT;
	animation = 1;
	cnt_animation = 0;
	pacman_stopped = 0;
	idxCurrentRail = 33;
}

SDL_Surface* Pacman::get_Surface() const {
	return pacman_sf;
}

int Pacman::touch(Figur **ghost_array) const{
	int x_left_pacman = this->x;
	int y_up_pacman = this->y;
	int x_right_pacman = this->x + this->get_Surface()->w;
	int y_down_pacman = this->y + this->get_Surface()->h;
	int touch_result = 0;
	
    for(int i = 0; i <= 3; i++) {
    	int x_real_ghost = ghost_array[i]->x + (int)(ghost_array[i]->get_Surface()->w * 0.5);
		int y_real_ghost = ghost_array[i]->y + (int)(ghost_array[i]->get_Surface()->h * 0.5);
		if((x_real_ghost >= x_left_pacman) && (x_real_ghost <= x_right_pacman) && (y_real_ghost >= y_up_pacman) && (y_real_ghost <= y_down_pacman))
			touch_result = ghost_array[i]->touched();
			if(touch_result != 0)
				return 1;
	}			
	return 0;
}

void Pacman::check_eat_pills(Figur **ghost_array) {
	if(was_moving() && !is_dying()){
		int tmp_last_x = this->last_x;
		if (this->y == 215) {
			if (this->last_x < 310 && this->x > 310)  // tunnel crossed - gone out to the left and coming in from the right
				tmp_last_x = 516;  // if we did not pretend something like this, a pill would be eaten that Pacman did not really pass
			if (this->last_x > 310 && this->x < 310)  // tunnel crossed - gone out to the right and coming in from the left
				tmp_last_x = 99;  // if we did not pretend something like this, a pill would be eaten that Pacman did not really pass
		}
		for(int i = 0; i < this->labyrinth->NUMBER_PILLS; i++) {
			if(this->labyrinth->pillen[i].sichtbar && ((this->labyrinth->pillen[i].x - 10) >= least(this->x,tmp_last_x)) && ((this->labyrinth->pillen[i].x - 10) <= greatest(this->x,tmp_last_x)) && ((this->labyrinth->pillen[i].y - 10) >= least(this->y,this->last_y)) && ((this->labyrinth->pillen[i].y - 10) <= greatest(this->y,this->last_y))) {
				cnt_slow = 15;
				this->labyrinth->hidePill(i);
				this->set_speed(PACMAN_V_SLOW);
				this->labyrinth->addScore(10);
				this->labyrinth->decreasePills();
				this->labyrinth->getSounds()->munch();
				if(this->labyrinth->pillen[i].superpille) {
					for(int j = 0; j < 4; ++j) {
						if(ghost_array[j]->get_hunter() != NONE)  // eaten ghosts still have to return to the castle
							ghost_array[j]->set_hunter(PACMAN);
					}
					this->labyrinth->startHuntingMode();
					this->labyrinth->addScore(40);
				}
				break;
			}	
		}	
		// only if pacman really has moved
		if(cnt_slow > 0)
			cnt_slow--; 
		if(cnt_slow <= 0)
			this->set_speed(PACMAN_V_FAST); // make pacman fast again

		// fruit eaten?
		if(labyrinth->fruitIsDisplayed() && this->y == 257 && (this->x>=310 && this->last_x<=310 || this->x<=310 && this->last_x>=310)) {
			labyrinth->playSoundFruit();
			labyrinth->hideFruit();
			labyrinth->addScore(labyrinth->getFruitBonus(), 310 + 10, 257 + 10);
			this->setVisibility(0);
			labyrinth->sleep(400);
		}
	}
}

void Pacman::set_dying(int dying) {
	this->dying = dying;
}

int Pacman::is_dying() const {
	return dying;
}

int Pacman::die_animation() {
	if(dying > 1) 
		return dying--;
	else {
		if(!die_counter)
			labyrinth->playSoundDying();
		this->die_pic(die_counter++);
		if(die_counter == 16) {
			die_counter = 0;
			dying = 0;
		}
		return die_counter;
	}
}

void Pacman::addUpdateRect() {
	screen->AddUpdateRects(least(x,last_x), least(y,last_y), pacman_sf->w + abs(x-last_x), pacman_sf->h + abs(y-last_y));
}

void Pacman::drawLives() {
	const int X_POS = 530;
	const int Y_POS = 110;
	const int X_OFFSET = pacman_links_1->w + 3;
	for (int i = 0; i < remainingLives-1; ++i)
		screen->draw(pacman_links_1, X_POS+(i*X_OFFSET), Y_POS);
	screen->AddUpdateRects(X_POS, Y_POS, (remainingLives*X_OFFSET)+pacman_links_1->w, pacman_links_1->h);
}

void Pacman::addLives(int num) {
	remainingLives += num;
}

int Pacman::getRemainingLives() {
	return remainingLives;
}

