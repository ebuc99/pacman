#ifndef GHOST_FIGUR_H
#define GHOST_FIGUR_H

class Ghost;  // this tells the following include files that Ghost is a class
#include "figur.h"
#include "screen.h"
#include "schiene.h"
#include <time.h>
#include "pacman_figur.h"

class Ghost : public Figur {
	public:
		/* constructor and destructor */
		Ghost(int init_x, int init_y, float init_v, uint16_t init_intelligence, 
		      uint16_t init_richtung, uint16_t init_up_down, uint16_t ghost_ident);
		~Ghost();

		/* draw ghost */
		void draw(Screen *screen);

		/* ghost animation */
		void animation(int cnt_pic);
		
		// defines the leader (reference for redrawing)
		void set_leader(uint16_t leader); 
		
		// overloaded move of class Figur
		void move(float ms, int direction, float max_links = 999, float max_oben = 999, float max_rechts = 999, float max_unten = 999);
		
		// has the leader moved?
		static uint16_t was_moving_leader;
		
		// returns the relative position between pacman and the ghost, coded like this: 0 = left, 1 = up, 2 = right, 3 = down
		uint16_t relative_pos_pacman(Pacman *pacman);
		
		// This is the A.I. of the ghosts: with a defined probability, they move to the direction where pacman is.
		uint16_t choose_direction(uint16_t * sammel_richtung, uint16_t richtung_pacman, int sammel_counter, uint16_t intelligence);
		
		// moves a ghost on the defined rails
		void move_on_rails(Pacman *pacman, float ms, int anz_schienen, Schiene **ar_s);
		
		// returns the intelligence of the ghost
		uint16_t get_intelligence() const;
		
		// reset the ghost
		void reset();

		/* Idents for the ghosts */
		static const uint16_t BLINKY = 0;
		static const uint16_t PINKY = 1;
		static const uint16_t INKY = 2;
		static const uint16_t CLYDE = 3;

		/* Ghost Surface */
		SDL_Surface *ghost_sf;
		
	private:
		uint16_t its_leader;
		uint16_t up_down;
		uint16_t initial_richtung;
		uint16_t initial_intelligence;
		uint16_t initial_up_down;
		SDL_Surface *ghost_1, *ghost_2, *ar_ghost[2];
		SDL_Surface *augen_0, *augen_1, *augen_2, *augen_3;
		
};
#endif