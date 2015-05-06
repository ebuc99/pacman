#ifndef GHOST_FIGUR_H
#define GHOST_FIGUR_H

#include "figur.h"
#include "rail.h"
#include <time.h>

class Ghost : public Figur {
	public:
		/* constructor and destructor */
		Ghost(int init_x, int init_y, int init_intelligence, 
		      int init_richtung, int init_up_down, int ghost_ident,
		      Screen *screen, Labyrinth *labyrinth);
		~Ghost();

		/* draw ghost */
		void draw();

		// animate the ghost, i.e. switch to the next image of the animation array
		void animation();
		
		// defines the leader (reference for redrawing)
		void set_leader(int leader); 
		void set_leader();
		
		// overloaded move of class Figur
		void move(float ms, int direction, float max_links = 999, float max_oben = 999, float max_rechts = 999, float max_unten = 999);
		void move(int moving, Figur *pacman, float(ms));
		
		// has the leader moved?
		static int was_moving_leader;
		
		// Returns the direction that has to be taken to reach the given target point (e.g. pacman's current position).
		// The result is coded like this: 0 = left, 1 = up, 2 = right, 3 = down
		int direction_to_point(int target_x, int target_y);
		int alternative_direction_to_point(int target_x, int target_y);  // Returns an alternative direction. Use it, if the exact one is not available.
		
		// This is the A.I. of the ghosts: with a defined probability, they move to the direction where pacman is.
		int choose_direction(int * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence);

		// moves a ghost on the defined rails
		void move_on_rails(Figur *pacman, float ms, int anz_schienen, Rail **ar_s);
		
		// returns the intelligence of the ghost
		int get_intelligence() const;
		
		// reset the ghost
		void reset();

		// this function registers the ghosts' graphics for redrawing, but only if they have changed
		void AddUpdateRects_ghost();

		// hunting or hunted
		Figur::Hunter get_hunter() const;
		void set_hunter(Hunter hunter);

		// Callback method when pacman has touched this ghost. Returns 1 if this was dangerous for
		// pacman, 0 otherwise.
		int touched();

		// announce the end of the hunting mode by blinking
		void blink();

		/* Idents for the ghosts */
		enum Ghosts {BLINKY, PINKY, INKY, CLYDE};

		// return the surface
		SDL_Surface* get_Surface() const;

		// set ghost_array
		void setGhostArray(Ghost **ghost_array);
		
		Ghosts getGhostIdent() const;
		
	private:
		static const float GHOSTS_V = 0.18f; // normal speed of the ghosts
		int its_leader;
		int up_down;
		int initial_richtung;
		int initial_intelligence;
		int initial_up_down;
		int num_animation_frames;
		int idx_animation;
		int ghost_ident;
		SDL_Surface *ghost_sf;
		SDL_Surface *ghost_1, *ghost_2, *ar_ghost[4];
		SDL_Surface *augen_0, *augen_1, *augen_2, *augen_3;
		SDL_Surface *escape_1, *escape_2, *escape_white_1, *escape_white_2;
		Ghost **ghost_array;
};
#endif
