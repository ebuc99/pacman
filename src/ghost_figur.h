#ifndef GHOST_FIGUR_H
#define GHOST_FIGUR_H

#include "figur.h"
#include "schiene.h"
#include <time.h>
#include "pacman_figur.h"

class Ghost : public Figur {
	public:
		/* constructor and destructor */
		Ghost(int init_x, int init_y, float init_v, int init_intelligence, 
		      int init_richtung, int init_up_down, unsigned short int ghost_ident);
		~Ghost();

		/* draw ghost */
		void draw(SDL_Surface *screen, int moving);

		/* ghost animation */
		void animation(int cnt_pic);
		
		// defines the leader (reference for redrawing)
		void set_leader(int leader); 
		
		// overloaded move of class Figur
		void move(float ms, int direction, float max_links = 999, float max_oben = 999, float max_rechts = 999, float max_unten = 999);
		
		// has the leader moved?
		static int was_moving_leader;
		
		// returns the relative position between pacman and the ghost, coded like this: 0 = left, 1 = up, 2 = right, 3 = down
		int relative_pos_pacman(Pacman *pacman);
		
		// This is the A.I. of the ghosts: with a defined probability, they move to the direction where pacman is.
		int choose_direction(int * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence);
		
		// moves a ghost on the defined rails
		void move_on_rails(Pacman *pacman, float ms, int anz_schienen, Schiene **ar_s);
		
		// returns the intelligence of the ghost
		int get_intelligence() const;
		
		// reset the ghost
		void reset();

		/* Idents for the ghosts */
		static const unsigned short int BLINKY = 0;
		static const unsigned short int PINKY = 1;
		static const unsigned short int INKY = 2;
		static const unsigned short int CLYDE = 3;

		/* Ghost Surface */
		SDL_Surface *ghost_sf;
		
	private:
		unsigned short int its_leader;
		unsigned short int up_down;
		unsigned short int initial_richtung;
		unsigned short int initial_intelligence;
		unsigned short int initial_up_down;
		SDL_Surface *ghost_1, *ghost_2, *ar_ghost[2];
		SDL_Surface *augen_0, *augen_1, *augen_2, *augen_3;
		
};
#endif