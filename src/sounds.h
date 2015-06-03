#ifndef SOUNDS_H
#define SOUNDS_H
#include <SDL/SDL.h>
#define SOUNDS 2

static struct sample {
			Uint8 *data;
			Uint32 pos;
			Uint32 len;
		} sounds[SOUNDS];
void MyAudioCallback(void *udata, Uint8 *stream, int len);

class Sounds {
	public:
		Sounds();
		~Sounds();
		void munch();
	protected:
		bool munch_toggle;
		void init_audio(void);
		void deinit_audio(void);
		void audioPlay(const char* file);
};

#endif