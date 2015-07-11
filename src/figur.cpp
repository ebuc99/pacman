#include "figur.h"
#include "math.h"


Figur::Figur(int init_x, int init_y, int init_v,
             Screen *screen, Labyrinth *labyrinth):
	initial_x(init_x),
	initial_y(init_y),
	initial_v(init_v),
	visible(1) {
	x = init_x;
	y = init_y;
	last_x = init_x;
	last_y = init_y;
	cur_x = init_x << 10;
	cur_y = init_y << 10;
	dx = init_v;
	dy = init_v;
	this->screen = screen;
	this->labyrinth = labyrinth;
}

Figur::~Figur() {
}

void Figur::move_left(int ms, int stop_at) {
	last_x = x;
	last_y = y;
	cur_x = greatest(cur_x - dx*ms, stop_at << 10);
	x = cur_x >> 10;
	direction = LEFT;
}

void Figur::move_up(int ms, int stop_at) {
	last_x = x;
	last_y = y;
	cur_y = greatest(cur_y - dy*ms, stop_at << 10);
	y = cur_y >> 10;
	direction = UP;
}

void Figur::move_right(int ms, int stop_at) {
	last_x = x;
	last_y = y;
	cur_x = least(cur_x + dx*ms, stop_at << 10);
	x = cur_x >> 10;
	direction = RIGHT;
}

void Figur::move_down(int ms, int stop_at) {
	last_x = x;
	last_y = y;
	cur_y = least(cur_y + dy*ms, stop_at << 10);
	y = cur_y >> 10;
	direction = DOWN;
}

void Figur::move_dir(int ms, Direction direction) {
	if(direction == LEFT)
		move_left(ms);
	if(direction == UP)
		move_up(ms);
	if(direction == RIGHT)
		move_right(ms);
	if(direction == DOWN)
		move_down(ms);		
}
void Figur::set_speed(int v) {
	dx = v;
	dy = v;
}

int Figur::get_speed() const {
	return dx;
}

bool Figur::was_moving() {
	if((x != last_x) || (y != last_y))
		return true;
	else
		return false;
}

void Figur::parking() {
	last_x = x;
	last_y = y;

}

Figur::Direction Figur::get_direction() const {
	return direction;
}

void Figur::set_direction(Direction direction) {
	this->direction = direction;
}

void Figur::set_position(int new_x, int new_y) {
	last_x = x;
	last_y = y;
	x = new_x;
	y = new_y;
	cur_x = new_x << 10;
	cur_y = new_y << 10;
}

void Figur::set_stop(int stop) {
	if(stop){
		last_speed = this->get_speed();
		this->set_speed(0);
	} 
	else {
		this->set_speed(last_speed);
	}
}

int Figur::least(int a, int b) {
	return (a < b) ? a : b;
}

int Figur::greatest(int a, int b) {
	return (a > b) ? a : b;
}

/*SDL_Surface* Figur::LoadSurface(const char *filename, int transparent_color = -1) {
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
}*/

void Figur::reset() {
}

Figur::Hunter Figur::get_hunter() const {
	return NONE;
}

void Figur::set_hunter(Hunter hunter) {
		this->hunter = hunter;
}

void Figur::setVisibility(bool v) {
	int prev_value = this->visible;
	this->visible = (v == false) ? false : true;
	if (prev_value != this->visible)
		draw();
		addUpdateRect();
}

