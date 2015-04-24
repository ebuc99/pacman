#ifndef LABYRINTH_H
#define LABYRINTH_H

class Labyrinth;
#include "screen.h"
#include "pille.h"

class Labyrinth {
	private: 
		static const int NUMBER_RAILS = 50; // number of rails
		static const int NUMBER_RAILS_PILLS = 37; // number of pill-filled rails
		int cnt_pill_animation; 
		Rail *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11, 
				*s12, *s13, *s14, *s15, *s16, *s17, *s18, *s19, *s20, *s21, 
				*s22, *s23, *s24, *s25,	*s26, *s27, *s28, *s29, *s30, *s31,
				*s32, *s33, *s34, *s35, *s36, *s37, *s38, *s39, *s40, *s41,
				*s42, *s43, *s44, *s45, *s46, *s47, *s48, *s49;
		SDL_Surface *pille, *superpille, *ar_superpille[5];
		
		//very bad, because the same method is in figur (and pacman)
		// it must be in a basic class in future
		SDL_Surface *LoadSurface(const char *filename, int transparent_color = -1);
	public:
		//constructor and destructor
		Labyrinth(Screen *screen);
		~Labyrinth();

		// At the left and right of the tunnel, there are two black rectangles,
		// below which pacman and the ghosts will disappear if they leave the
		// level to the left or right.
		void draw_blocks();

		/* initially, throw some pills across the level */
		void init_pillen();

		/* draw pills, but only if Blinky has moved */
		void draw_pillen();

		void pill_animation();

		int number_rails() const;

		Screen *screen;
		int sdl_init_error;

		static const int NUMBER_PILLS = 246; // number of pills
		Pille pillen[NUMBER_PILLS];
		
		Rail *array_rails[NUMBER_RAILS];
		Rail *array_rails_pills[NUMBER_RAILS_PILLS];
};
#endif