#include "labyrinth.h"

Labyrinth::Labyrinth(SDL_Surface *screen){
	this->screen = screen;
}

Labyrinth::~Labyrinth(){
}

void Labyrinth::draw_blocks() {
  	SDL_Rect b1, b2;
  	b1.x = 100;
  	b1.y = 215;
  	b1.w = 30;
  	b1.h = 30;
  
 	b2.x = 515;
  	b2.y = 215;
  	b2.w = 30;
  	b2.h = 30;
  
  	SDL_FillRect(this->screen, &b1, SDL_MapRGB(this->screen->format, 0, 0, 0));
  	SDL_FillRect(this->screen, &b2, SDL_MapRGB(this->screen->format, 0, 0, 0));
}
	