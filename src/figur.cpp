#include "figur.h"
#include "math.h"
#include <SDL/SDL.h>


Figur::Figur(int init_x, int init_y, float init_v):
	initial_x(init_x),
	initial_y(init_y),
	initial_v(init_v) {
	x = init_x;
	y = init_y;
	last_x = init_x;
	last_y = init_y;
	cur_x = init_x;
	cur_y = init_y;
	dx = init_v;
	dy = init_v;
}

Figur::~Figur() {
}

void Figur::move_left(float ms, float max_step) {
	last_x = x;
	last_y = y; 
	cur_x = (cur_x - least(dx*ms, max_step));
	x = int(cur_x);
	richtung = 0;
}

void Figur::move_up(float ms, float max_step) {
	last_x = x;
	last_y = y; 
	cur_y = (cur_y - least(dy*ms, max_step));
	y = int(cur_y); 
	richtung = 1;
}

void Figur::move_right(float ms, float max_step) {
	last_x = x;
	last_y = y; 
	cur_x = (cur_x + least(dx*ms, max_step));
	x = int(cur_x); 
	richtung = 2;
}

void Figur::move_down(float ms, float max_step) {
	last_x = x;
	last_y = y;
	cur_y = (cur_y + least(dy*ms, max_step));
	y = int(cur_y); 
	richtung = 3;
}

void Figur::move(float ms, int direction) {
	if(direction == 0)
		move_left(ms);
	if(direction == 1)
		move_up(ms);
	if(direction == 2)
		move_right(ms);
	if(direction == 3)
		move_down(ms);		
}
void Figur::set_speed(float v) {
	dx = v;
	dy = v;
}

float Figur::get_speed() const {
	return dx;
}

int Figur::was_moving() {
	if((x != last_x) || (y != last_y))
		return 1;
	else
		return 0;
}

void Figur::parking() {
	last_x = x;
	last_y = y;

}

int Figur::get_richtung() const {
	return richtung;
}

void Figur::set_richtung(int p_richtung) {
	richtung = p_richtung;
}

float Figur::least(float a, float b) {
	if(a < b) 
		return a;
	else 
		return b;
}

SDL_Surface* Figur::LoadSurface(const char *filename, int transparant_color = -1) {
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
