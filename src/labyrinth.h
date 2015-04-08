#include "pacman_figur.h"
#include "ghost_figur.h"

class Labyrinth {
	public:
		//constructor and destructor
		Labyrinth();
		~Labyrinth();

		// At the left and right of the tunnel, there are two black rectangles,
		// below which pacman and the ghosts will disappear if they leave the
		// level to the left or right.
		void draw_blocks();

		SDL_Surface *screen;
		int sdl_init_error;
};