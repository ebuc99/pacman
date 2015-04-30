#ifndef GHOST_FIGUR_H
#define GHOST_FIGUR_H

#include "figur.h"
#include "rail.h"
#include <time.h>

class Ghost : public Figur {
	public:
		/* constructor and destructor */
		Ghost(int init_x, int init_y, int init_intelligence, 
		      int init_richtung, int init_up_down, int ghost_ident);
		~Ghost();

		/* draw ghost */
		void draw();

		/* ghost animation */
		void animation(int cnt_pic);
		
		// defines the leader (reference for redrawing)
		void set_leader(int leader); 
		
		// overloaded move of class Figur
		void move(float ms, int direction, float max_links = 999, float max_oben = 999, float max_rechts = 999, float max_unten = 999);
		void move(int moving, Figur *pacman, float(ms));
		
		// has the leader moved?
		static int was_moving_leader;
		
		// returns the relative position between pacman and the ghost, coded like this: 0 = left, 1 = up, 2 = right, 3 = down
		int relative_pos_pacman(Figur *pacman);
		
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

		/* Idents for the ghosts */
		static const int BLINKY = 0;
		static const int PINKY = 1;
		static const int INKY = 2;
		static const int CLYDE = 3;

		// return the surface
		SDL_Surface* get_Surface() const;
		
	private:
		static const float GHOSTS_V = 0.18f; // normal speed of the ghosts
		int its_leader;
		int up_down;
		int initial_richtung;
		int initial_intelligence;
		int initial_up_down;
		SDL_Surface *ghost_sf;
		SDL_Surface *ghost_1, *ghost_2, *ar_ghost[2];
		SDL_Surface *augen_0, *augen_1, *augen_2, *augen_3;
		SDL_Surface *escape_1, *escape_2;
		
};
#endif
