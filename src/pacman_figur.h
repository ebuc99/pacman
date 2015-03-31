#ifndef PACMAN_FIGUR_H
#define PACMAN_FIGUR_H

#include "figur.h"
#include "schiene.h"
class Pacman : public Figur {
	public:
		/* Konstruktor und Destruktor */
		Pacman(int init_x, int init_y, float init_v, int init_wechsel_rate);
		~Pacman();
		
		/* Bewegung in alle vier Richtungen */
		void move_left(float ms, float max_step = 999);
		void move_up(float ms, float max_step = 999);
		void move_right(float ms, float max_step = 999);
		void move_down(float ms, float max_step = 999);
		
		/* meldet, ob pacman auf ein neues Bild gestellt werden kann */
		int wechsel();
		
		/* Bewegt den Geist auf den Schiene */ 
		void move_on_rails(float ms, int anz_schienen, Schiene *ar_s);
		
		/* meldet, ob pacman grad steht */
		int is_pacman_stopped();
		
		/* die Richtung, die pacman als nächstes einschlagen wird */
		int richtung_pre;
		
		/* Pacman wieder zurücksetzen */
		void reset();
		
		/* Pacman stirbt gerade */
		unsigned short int is_dying;
	private:
		unsigned short int pacman_stopped; 
		unsigned short int is_wechsel;
};
#endif
