#ifndef SOUNDS_H
#define SOUNDS_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "platform.h"

class Sounds {
	public:
		static Sounds *getInstance();
		static void cleanUpInstance();
		bool isEnabled() const;
		void setEnabled(bool newValue = true);
		void toggleEnabled();
		enum SingleSounds{DYING, EXTRA_MAN, FRUIT, EAT_GHOST};
		void pause_all();
		void resume_all();
		void munch();
		void intro();
		void siren_start();
		void superpill_start();
		void eat_ghost_start();
		void eat_ghost_stop();
		void music_stop();
		void channelStop();
		void playSingleSound(SingleSounds singlesounds);
	protected:
		static Sounds *instance;
		Sounds();
		~Sounds();
		bool enabled;
		Mix_Chunk *chunk_munch_a = NULL;
		Mix_Chunk *chunk_munch_b = NULL;
		int channel_munch;
		bool munch_toggle;
		Mix_Music *music_intro = NULL;
		Mix_Music *music_siren_slow = NULL;
		Mix_Music *music_superpill_loop = NULL;
		Mix_Music *music_eat_ghost = NULL;
		Mix_Chunk *chunk_dying = NULL;
		Mix_Chunk *chunk_extra_man = NULL;
		Mix_Chunk *chunk_fruit = NULL;
		Mix_Chunk *chunk_eat_ghost = NULL;
		int eat_ghost_cnt;
};

#endif
