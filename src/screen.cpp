#include "screen.h"

Screen::Screen() {
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL initialization failed: %s\n", SDL_GetError());
                this->sdl_init_error = EXIT_FAILURE;
        }
	atexit(SDL_Quit);
	this->screen_sf = SDL_SetVideoMode(680, 512, 24, SDL_HWSURFACE);
        if(this->screen_sf == 0) {
                printf("Setting video mode failed: %s\n",SDL_GetError());
                this->sdl_init_error = EXIT_FAILURE;
        }
	SDL_WM_SetCaption("Pacman", "");
	rect_num = 0;
}

Screen::~Screen() {
}

void Screen::AddUpdateRects(int x, int y, int w, int h) {
	if (x < 0) {
         	w += x;
        	x = 0;
    	}
    	if (y < 0) {
        	h += y;
        	y = 0;
   	}
	if (x + w > this->screen_sf->w)
       	 	w = this->screen_sf->w - x;
    	if (y + h > this->screen_sf->h)
        	h = this->screen_sf->h - y;
    	if (w <= 0 || h <= 0)
        	return;
    	rects[rect_num].x = x;
    	rects[rect_num].y = y;
    	rects[rect_num].w = w;
    	rects[rect_num].h = h;
    	rect_num++;
}

void Screen::Refresh(int moving) {
	if(moving){
		SDL_UpdateRects(this->screen_sf, this->rect_num, this->rects);
		this->rect_num = 0;
	}
}

void Screen::draw_static_content(SDL_Surface *surface, int x, int y, int moving, int force) {
	static unsigned short int  temp_force = 0;
	if(force)
		temp_force = force;
	if(moving && temp_force) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, this->screen_sf, &dest);
		this->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
		temp_force = 0;
	}
}

void Screen::draw_dynamic_content(SDL_Surface *surface, int x, int y, int moving) {
	if(moving) {
		SDL_Rect dest;
		dest.x = x; 
		dest.y = y; 
		SDL_BlitSurface(surface, NULL, this->screen_sf, &dest);
		this->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
	}
}
