#include "figur.h"
#include "schiene.h"
#include <time.h>
#include "pacman_figur.h"

class Ghost : public Figur {
	public:
		/* Konstruktor und Destruktor */
		Ghost(int init_x, int init_y, float init_v, int init_intelligence, 
		      int init_richtung, int init_up_down, unsigned short int ghost_ident);
		~Ghost();

		/* draw ghost */
		void draw(SDL_Surface *screen, int moving);

		/* ghost animation */
		void animation(int cnt_pic);
		
		/* setzt den Leader (maßgeblich für das Neuzeichnen)*/
		void set_leader(int leader); 
		
		/* Überladenes move von Figur */
		void move(float ms, int direction, float max_links = 999, float max_oben = 999, float max_rechts = 999, float max_unten = 999);
		
		/* wurde der Leader bewegt? */
		static int was_moving_leader;
		
		/* liefert die relative Positon von Pacman zum Geist in der Kodierung 0 = links, 1 = oben, 2 = rechts, 3 = unten */
		int relative_pos_pacman(Pacman *pacman);
		
		/* Hier steckt die "künstliche Intelligenz" der Geister. Sie laufen mit einer Wahrscheinlichkeit dort hin,
   		   wo pacman ist. */ 
		int choose_direction(int * sammel_richtung, int richtung_pacman, int sammel_counter, int intelligence);
		
		/* Bewegt den Geist auf den Schiene */
		void move_on_rails(Pacman *pacman, float ms, int anz_schienen, Schiene *ar_s);
		
		/* liefert die Intelligenz des Geistes */
		int get_intelligence() const;
		
		/* Geist wieder zurücksetzen */
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
