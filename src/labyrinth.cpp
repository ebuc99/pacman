#include "labyrinth.h"

Labyrinth::Labyrinth(){
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL initialization failed: %s\n", SDL_GetError());
                this->sdl_init_error = EXIT_FAILURE;
        }
	atexit(SDL_Quit);
	this->screen = SDL_SetVideoMode(680, 512, 24, SDL_HWSURFACE);
        if(this->screen == 0) {
                printf("Setting video mode failed: %s\n",SDL_GetError());
                this->sdl_init_error = EXIT_FAILURE;
        }
	SDL_WM_SetCaption("Pacman", "");
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
	