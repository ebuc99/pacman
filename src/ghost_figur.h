#include "figur.h"
#include "schiene.h"
#include <time.h>
#include "pacman_figur.h"

class Ghost : public Figur {
	public:
		/* Konstruktor und Destruktor */
		Ghost(int init_x, int init_y, float init_v, int init_intelligence, int init_richtung, int init_up_down);
		~Ghost();
		
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
	private:
		unsigned short int its_leader;
		unsigned short int up_down;
		unsigned short int initial_richtung;
		unsigned short int initial_intelligence;
		unsigned short int initial_up_down;
		
};
