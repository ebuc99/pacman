#ifndef PACMAN_FIGUR_H
#define PACMAN_FIGUR_H

#include "figur.h"
#include "schiene.h"
class Pacman : public Figur {
	public:
		/* Konstruktor und Destruktor */
		Pacman(int init_x, int init_y, float init_v, int init_wechsel_rate);
		~Pacman();
		/* draw pacman */
		void draw(SDL_Surface* screen);
		
		/* Bewegung in alle vier Richtungen */
		void move_left(float ms, float max_step = 999);
		void move_up(float ms, float max_step = 999);
		void move_right(float ms, float max_step = 999);
		void move_down(float ms, float max_step = 999);

		/* pacman all direction animation */
		void left_pic(int cnt_pic);
		void up_pic(int cnt_pic);
		void right_pic(int cnt_pic);
		void down_pic(int cnt_pic);
		
		/* for pacman die animation */
		void die_pic(int cnt_pic);
		
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

		SDL_Surface *pacman;
	private:
		unsigned short int pacman_stopped; 
		unsigned short int is_wechsel;
		int ct_pm = 0;
		SDL_Surface *pacman_normal;
		SDL_Surface *pacman_links_1, *pacman_links_2, *ar_pacman_links[4];
		SDL_Surface *pacman_oben_1, *pacman_oben_2, *ar_pacman_oben[4];
		SDL_Surface *pacman_rechts_1, *pacman_rechts_2, *ar_pacman_rechts[4];
		SDL_Surface *pacman_unten_1, *pacman_unten_2, *ar_pacman_unten[4];
		SDL_Surface *ar_pacman_die[13];
};
#endif
