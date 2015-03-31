#include "pacman_figur.h"
#include "math.h"

Pacman::Pacman(int init_x, int init_y, float init_v, int init_wechsel_rate):
	Figur(init_x, init_y, init_v),
	is_wechsel(0),
	pacman_stopped(0),
	is_dying(0) {
    wechsel_rate = init_wechsel_rate;
	wechsel_x = init_x;
	wechsel_y = init_y;
	richtung = 0;
}

Pacman::~Pacman() {
}

void Pacman::move_left(float ms, float max_step) {
	last_x = x;
	last_y = y;
	cur_x = (cur_x - least(dx*ms, max_step));
	x = int(cur_x); 
	if ((wechsel_rate > 0) && ((fabs(x - wechsel_x) >= wechsel_rate)||(fabs(y - wechsel_y) >= wechsel_rate))) {
		wechsel_x = x;
		wechsel_y = y;
		is_wechsel = 1;
	}
	richtung = 0;
}

void Pacman::move_up(float ms, float max_step) {
	last_x = x;
	last_y = y;
	cur_y = (cur_y - least(dy*ms, max_step));
	y = int(cur_y); 
	if ((wechsel_rate > 0) && ((fabs(x - wechsel_x) >= wechsel_rate)||(fabs(y - wechsel_y) >= wechsel_rate))) {
		wechsel_x = x;
		wechsel_y = y;
		is_wechsel = 1;
	}
	richtung = 1;
}

void Pacman::move_right(float ms, float max_step) {
	last_x = x;
	last_y = y;
	cur_x = (cur_x + least(dx*ms, max_step));
	x = int(cur_x); 
	if ((wechsel_rate > 0) && ((fabs(x - wechsel_x) >= wechsel_rate)||(fabs(y - wechsel_y) >= wechsel_rate))) {
		wechsel_x = x;
		wechsel_y = y;
		is_wechsel = 1;
	}
	richtung = 2;
}

void Pacman::move_down(float ms, float max_step) {
	last_x = x;
	last_y = y;
	cur_y = (cur_y + least(dy*ms, max_step));
	y = int(cur_y); 
	if ((wechsel_rate > 0) && ((fabs(x - wechsel_x) >= wechsel_rate)||(fabs(y - wechsel_y) >= wechsel_rate))) {
		wechsel_x = x;
		wechsel_y = y;
		is_wechsel = 1;
	}
	richtung = 3;
}

int Pacman::wechsel() {
	if (is_wechsel) {
		is_wechsel = 0;
		return 1;
	}
	else
		return 0;
}

void Pacman::move_on_rails(float ms, int anz_schienen, Schiene *ar_s) {
	int i;
	int check_move = 0;
	if(this->get_richtung() != this->richtung_pre) {
		for(i = 0; i <= anz_schienen - 1; i++) {
			if((this->richtung_pre == 0) && (this->x > ar_s[i].x1) && (this->y == ar_s[i].y1) && (this->y == ar_s[i].y2) && (this->x <= ar_s[i].x2)) {
				if(this->get_richtung() != this->richtung_pre)
					this->is_wechsel = 1;
				this->set_richtung(this->richtung_pre);
			}
			if((this->richtung_pre == 1) && (this->y > ar_s[i].y1) && (this->x == ar_s[i].x1) && (this->x == ar_s[i].x2) && this->y <= ar_s[i].y2) {
				if(this->get_richtung() != this->richtung_pre)
					this->is_wechsel = 1;
				this->set_richtung(this->richtung_pre);
			}
			if((this->richtung_pre == 2) && (this->x < ar_s[i].x2) && (this->y == ar_s[i].y1) && (this->y == ar_s[i].y2) && (this->x >= ar_s[i].x1)) {
				if(this->get_richtung() != this->richtung_pre)
					this->is_wechsel = 1;
				this->set_richtung(this->richtung_pre);
			}
			if((this->richtung_pre == 3) && (this->y < ar_s[i].y2) && (this->x == ar_s[i].x1) && (this->x == ar_s[i].x2) && (this->y >= ar_s[i].y1)) {
				if(this->get_richtung() != this->richtung_pre)
					this->is_wechsel = 1;
				this->set_richtung(this->richtung_pre);
			}
		}
	}
	for(i = 0; i <= anz_schienen - 1; i++) {	
		// zuerst den Mittelgang
		if((this->richtung_pre != 2) && (this->x <= 100) && (this->y == 215)) {
	 		this->x = 515;
	 		this->cur_x = 515;
	 		this->richtung_pre = 0;
	 		break;
		}
		if((this->richtung_pre != 0) && (this->x >= 515) && (this->y == 215)) {
	 		this->x = 100;
	 		this->cur_x = 100;
	 		this->richtung_pre = 2;
	 		break;
		}
		
		// jetzt auf den "normalen" Schienen
	  	if ((this->get_richtung() == 0) && (this->x > ar_s[i].x1) && (this->y == ar_s[i].y1) && (this->y == ar_s[i].y2) && (this->x <= ar_s[i].x2)) {
			this->move_left(ms, this->x - ar_s[i].x1);
			check_move = 1;
			break;
		}
		if ((this->get_richtung() == 1) && (this->y > ar_s[i].y1) && (this->x == ar_s[i].x1) && (this->x == ar_s[i].x2) && this->y <= ar_s[i].y2) {
			this->move_up(ms, this->y - ar_s[i].y1);
			check_move = 1;
			break;
		}
		if ((this->get_richtung() == 2) && (this->x < ar_s[i].x2) && (this->y == ar_s[i].y1) && (this->y == ar_s[i].y2) && (this->x >= ar_s[i].x1)) {
			this->move_right(ms, ar_s[i].x2 - this->x);
			check_move = 1;
			break;
		}
		if ((this->get_richtung() == 3) && (this->y < ar_s[i].y2) && (this->x == ar_s[i].x1) && (this->x == ar_s[i].x2) && (this->y >= ar_s[i].y1)) {
			this->move_down(ms, ar_s[i].y2 - this->y);
			check_move = 1;
			break;
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
	cur_x = initial_x;
	cur_y = initial_y;
	wechsel_x = initial_x;
	wechsel_y = initial_y;
	richtung = 0;
	richtung_pre = 0;
	is_wechsel = 1;
	pacman_stopped = 0;
}
