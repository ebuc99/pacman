#include "pacman_figur.h"
#include "ghost_figur.h"
#include "screen.h"
#include "pille.h"

class Labyrinth {
	private: 
		static const int NUMBER_RAILS = 50; // number of rails
		static const int NUMBER_RAILS_PILLS = 37; // number of pill-filled rails
		static const int NUMBER_PILLS = 246; // number of pills
		static const float PACMAN_V_FAST = 0.2f; // pacman's speed when not eating
		static const float PACMAN_V_SLOW = 0.18f; // pacman's speed while eating
		Pille pillen[NUMBER_PILLS];
		int cnt_slow; // number of loops that pacman will stay slow
		Rail *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11, 
				*s12, *s13, *s14, *s15, *s16, *s17, *s18, *s19, *s20, *s21, 
				*s22, *s23, *s24, *s25,	*s26, *s27, *s28, *s29, *s30, *s31,
				*s32, *s33, *s34, *s35, *s36, *s37, *s38, *s39, *s40, *s41,
				*s42, *s43, *s44, *s45, *s46, *s47, *s48, *s49;
		int less(int a, int b);
		int greater(int a, int b);
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
		void draw_pillen(SDL_Surface *pille, SDL_Surface *superpille);

		/* collision handling pacman <-> pills */
		void check_pillen(Pacman *pacman, int *punktestand);

		uint16_t number_rails() const;

		Screen *screen;
		int sdl_init_error;

		Rail *array_rails[NUMBER_RAILS];
		Rail *array_rails_pills[NUMBER_RAILS_PILLS];
};