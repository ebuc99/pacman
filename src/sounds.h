#ifndef SOUNDS_H
#define SOUNDS_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sounds {
	public:
		Sounds();
		~Sounds();
		void munch();
		void intro();
	protected:
		Mix_Chunk *chunk_munch_a = NULL;
		Mix_Chunk *chunk_munch_b = NULL;
		int channel_munch;
		bool munch_toggle;
		Mix_Chunk *chunk_intro = NULL;
};

#endif