#include "pacman_figur.h"
#include "math.h"

Pacman::Pacman(int init_x, int init_y, Screen *screen, Labyrinth *labyrinth, int lives):
	Figur(init_x, init_y, PACMAN_V_FAST, screen, labyrinth),
	animation(0),
	cnt_animation(0),
	pacman_stopped(0),
	dying(0),
	die_counter(0),
	remainingLives(lives)
{
    wechsel_rate = WECHSEL_RATE;
	wechsel_x = init_x;
	wechsel_y = init_y;
	direction = LEFT;
	direction_pre = LEFT;
	pacman_normal = Figur::LoadSurface("/usr/local/share/pacman/gfx/pacman.png", 255);
	pacman_links_1 = LoadSurface("/usr/local/share/pacman/gfx/pacman_links_1.png", 255);
    pacman_links_2 = LoadSurface("/usr/local/share/pacman/gfx/pacman_links_2.png", 255);
    pacman_oben_1 = LoadSurface("/usr/local/share/pacman/gfx/pacman_oben_1.png", 255);
    pacman_oben_2 = LoadSurface("/usr/local/share/pacman/gfx/pacman_oben_2.png", 255);
    pacman_rechts_1 = LoadSurface("/usr/local/share/pacman/gfx/pacman_rechts_1.png", 255);
    pacman_rechts_2 = LoadSurface("/usr/local/share/pacman/gfx/pacman_rechts_2.png", 255);
    pacman_unten_1 = LoadSurface("/usr/local/share/pacman/gfx/pacman_unten_1.png", 255);
    pacman_unten_2 = LoadSurface("/usr/local/share/pacman/gfx/pacman_unten_2.png", 255);
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
    
    ar_pacman_die[0] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_1.png", 255);
    ar_pacman_die[1] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_2.png", 255);
    ar_pacman_die[2] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_3.png", 255);
    ar_pacman_die[3] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_4.png", 255);
    ar_pacman_die[4] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_5.png", 255);
    ar_pacman_die[5] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_6.png", 255);
    ar_pacman_die[6] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_7.png", 255);
    ar_pacman_die[7] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_8.png", 255);
    ar_pacman_die[8] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_9.png", 255);
    ar_pacman_die[9] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_7.png", 255);
    ar_pacman_die[10] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_8.png", 255);
    ar_pacman_die[11] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_9.png", 255);
    ar_pacman_die[12] = LoadSurface("/usr/local/share/pacman/gfx/pacman_die_9.png", 255);
	
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
	direction = LEFT;
	checkAnimationChange();
}

void Pacman::move_up(int ms, int stop_at) {
	Figur::move_up(ms, stop_at);
	direction = UP;
	checkAnimationChange();
}

void Pacman::move_right(int ms, int stop_at) {
	Figur::move_right(ms, stop_at);
	direction = RIGHT;
	checkAnimationChange();
}

void Pacman::move_down(int ms, int stop_at) {
	Figur::move_down(ms, stop_at);
	direction = DOWN;
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
	int i;
	int check_move = 0;
	// check if the pre selected direction is
	// not equal to the current direction and pacman is on a cross road
	if(this->get_direction() != this->direction_pre) {
		for(i = 0; i <= anz_schienen - 1; i++) {
			if((this->direction_pre == LEFT) && (this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
				this->animation = 1;
				this->set_direction(this->direction_pre);
			}
			if((this->direction_pre == UP) && (this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
				this->animation = 1;
				this->set_direction(this->direction_pre);
			}
			if((this->direction_pre == RIGHT) && (this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
				this->animation = 1;
				this->set_direction(this->direction_pre);
			}
			if((this->direction_pre == DOWN) && (this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
				this->animation = 1;
				this->set_direction(this->direction_pre);
			}
		}
	}
	for(i = 0; i <= anz_schienen - 1; i++) {	
		// first, check the tunnel
		if((this->direction_pre != RIGHT) && (this->x <= 100) && (this->y == 215)) {
	 		this->x = 515;
	 		this->cur_x = this->x << 10;
	 		this->direction_pre = LEFT;
	 		break;
		}
		if((this->direction_pre != LEFT) && (this->x >= 515) && (this->y == 215)) {
	 		this->x = 100;
	 		this->cur_x = this->x << 10;
	 		this->direction_pre = RIGHT;
	 		break;
		}
		
		// now the "normal" rails
		if(this->get_direction() == LEFT) {
		  	if ((this->x > ar_s[i]->x1) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x <= ar_s[i]->x2)) {
				this->move_left(ms, ar_s[i]->x1);
				check_move = 1;
				break;
			}
		} else if(this->get_direction() == UP) {
			if ((this->y > ar_s[i]->y1) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && this->y <= ar_s[i]->y2) {
				this->move_up(ms, ar_s[i]->y1);
				check_move = 1;
				break;
			}
		} else if(this->get_direction() == RIGHT) { 
			if ((this->x < ar_s[i]->x2) && (this->y == ar_s[i]->y1) && (this->y == ar_s[i]->y2) && (this->x >= ar_s[i]->x1)) {
				this->move_right(ms, ar_s[i]->x2);
				check_move = 1;
				break;
			}
		} else if(this->get_direction() == DOWN) {
			if ((this->y < ar_s[i]->y2) && (this->x == ar_s[i]->x1) && (this->x == ar_s[i]->x2) && (this->y >= ar_s[i]->y1)) {
				this->move_down(ms, ar_s[i]->y2);
				check_move = 1;
				break;
			}
		}
	}
	
	if(!check_move) 
		pacman_stopped = 1;
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
	if(this->was_moving()){
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
				this->labyrinth->pillen[i].sichtbar = 0;
				this->set_speed(PACMAN_V_SLOW);
				this->labyrinth->addScore(10);
				this->labyrinth->decreasePills();
				this->labyrinth->playSoundMunch();
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
		this->die_pic(die_counter++);
		if(die_counter == 13) {
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

