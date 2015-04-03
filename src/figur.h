#ifndef FIGUR_H
#define FIGUR_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class Figur {
	public:
		/* Konstruktor und Destruktor */
		Figur(int init_x, int init_y, float init_v);
		~Figur();
		
		/* Position der Figur */
		int x, y; 
		
		/* letzten x und y Werte, wichtig bei Prüfung ob Pillen gegessen */
		int last_x, last_y; 
		
		/* Bewegung in alle vier Richtungen */
		void move_left(float ms, float max_step = 999);
		void move_up(float ms, float max_step = 999);
		void move_right(float ms, float max_step = 999);
		void move_down(float ms, float max_step = 999);
		
		/* Wrapper über die Funktionen zuvor */
		void move(float ms, int direction);
		
		/* setzt Geschwindigkeit der Figur */
		void set_speed(float v);
		
		/* liefert Geschwindigkeit */
		float get_speed() const;
		
		/* Wurde die Figur bewegt? Wichtig für Performancetuning! */
		int was_moving();
		
		/* Parkt die Figur */
		void parking();
		
		/* liefert die Richtung */
		int get_richtung() const;
		
		/* setzt die Richtung */
		void set_richtung(int p_richtung);
		
	protected:
		float cur_y;
		float cur_x;
		int wechsel_x;
		int wechsel_y;
		int wechsel_rate;
		float dx, dy;
		int richtung;
		int intelligence;
		float least(float a, float b);
		int initial_x;
		int initial_y;
		float initial_v;
		SDL_Surface *LoadSurface(const char *filename, int transparant_color);
};
#endif
