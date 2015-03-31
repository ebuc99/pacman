#ifndef PILLE_H
#define PILLE_H

class Pille {
	public: 
		/* Konstruktor und Destruktor */
		Pille();
		~Pille();
		
		/* Position der Pille */
		int x;
		int y;
		unsigned short int sichtbar;
		unsigned short int superpille;
};
#endif
