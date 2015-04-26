#ifndef PILLE_H
#define PILLE_H

class Pille {
	public: 
		// constructor and destructor
		Pille();
		~Pille();
		
		// position of the pill
		int x;
		int y;
		unsigned short int sichtbar;
		unsigned short int superpille;
};
#endif
