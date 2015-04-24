#include "screen.h"

Screen::Screen() {
	// initialize SDL
	this->sdl_init_error = 0;
	this->rect_num = 0;
	this->fullscreen = false;
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        this->sdl_init_error = EXIT_FAILURE;
    }
	atexit(SDL_Quit);
	this->screen_surface = SDL_SetVideoMode(680, 512, 24, SDL_HWSURFACE);
    if(this->screen_surface == 0) {
        printf("Setting video mode failed: %s\n",SDL_GetError());
        this->sdl_init_error = EXIT_FAILURE;
    }
	SDL_WM_SetCaption("Pacman", "");
	rect_num = 0;
}

Screen::~Screen() {
}

void Screen::AddUpdateRects(int x, int y, int w, int h) {
    if (rect_num >= 200)
        return;  // prevent index out of bounds problems
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > this->screen_surface->w)
        w = this->screen_surface->w - x;
    if (y + h > this->screen_surface->h)
        h = this->screen_surface->h - y;
    if (w <= 0 || h <= 0)
        return;
	rects[rect_num].x = (short int)x;
	rects[rect_num].y = (short int)y;
	rects[rect_num].w = (short int)w;
	rects[rect_num].h = (short int)h;
	rect_num++;
}

void Screen::Refresh() {
	SDL_UpdateRects(this->screen_surface, this->rect_num, this->rects);
	this->rect_num = 0;
}

void Screen::draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	SDL_Rect dest;
	dest.x = (short int)x; 
	dest.y = (short int)y; 
	SDL_BlitSurface(surface, NULL, this->screen_surface, &dest);
	this->AddUpdateRects(dest.x, dest.y, surface->w + 10, surface->h);
}

void Screen::draw(SDL_Surface* graphic, int offset_x, int offset_y) {
    if (0 == offset_x && 0 == offset_y) {
        SDL_BlitSurface(graphic, NULL, screen_surface, NULL);
    } else {
        SDL_Rect position;
        position.x = (short int)offset_x;
        position.y = (short int)offset_y;
        SDL_BlitSurface(graphic, NULL, screen_surface, &position);
    }
}

/*void Screen::draw(Ghost* ghost) {
    ghost->draw(this);
}

void Screen::draw(Pacman* pacman) {
    pacman->draw(this);
}*/

void Screen::setFullscreen(bool fs) {
    if (fs == fullscreen)
        return;  // the desired mode already has been activated, so do nothing
    SDL_Surface* newScreen;
    if(fs)
        newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_FULLSCREEN);
	else
        newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE);
    if (NULL != newScreen) {  // successful? NULL indicates failure
        this->screen_surface = newScreen;  // take it, but do not dispose of the old screen (says SDL documentation)
        this->AddUpdateRects(0, 0, this->screen_surface->w, this->screen_surface->h);
        // no Refresh() here, because at this moment nothing has been drawn to the new screen
        fullscreen = fs;
    }
}
