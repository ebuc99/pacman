#include "figur.h"
#include "math.h"


Figur::Figur(int init_x, int init_y, float init_v):
	initial_x(init_x),
	initial_y(init_y),
	initial_v(init_v) {
	x = init_x;
	y = init_y;
	last_x = init_x;
	last_y = init_y;
	cur_x = (float)init_x;
	cur_y = (float)init_y;
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
	return (a < b) ? a : b;
}

SDL_Surface* Figur::LoadSurface(const char *filename, int transparent_color = -1) {
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

SDL_Surface* Figur::get_Surface() const {
}

void Figur::reset() {
}

int Figur::less(int a, int b) {
	return (a < b) ? a : b;
}
int Figur::greater(int a, int b) {
	return (a > b) ? a : b;
}