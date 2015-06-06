#ifndef SOUNDS_H
#define SOUNDS_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sounds {
	public:
		Sounds();
		~Sounds();
		void pause_all();
		void resume_all();
		void munch();
		void intro();
		void siren_start();
		void siren_stop();
		void dying();
	protected:
		Mix_Chunk *chunk_munch_a = NULL;
		Mix_Chunk *chunk_munch_b = NULL;
		int channel_munch;
		bool munch_toggle;
		Mix_Chunk *chunk_intro = NULL;
		Mix_Chunk *chunk_siren_slow = NULL;
		int channel_siren;
		Mix_Chunk *chunk_dying = NULL;
};

#endif