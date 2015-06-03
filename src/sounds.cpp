#include "sounds.h"

Sounds::Sounds():
	munch_toggle(true){
	this->init_audio();
}

Sounds::~Sounds() {
	this->deinit_audio();
}

void Sounds::init_audio(void) {
	SDL_AudioSpec format;
	format.freq = 22050;
	format.format = AUDIO_S16;
	format.channels = 2;
	format.samples = 512;
	format.callback = MyAudioCallback;
	format.userdata = NULL;
	// Audiogerät öffnen und Abspielen beginnen
	if(SDL_OpenAudio(&format, NULL) < 0) {
		printf("Aufiogerät konnte nicht geöffnet werden: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_PauseAudio(0);
}

void Sounds::deinit_audio(void) {
	SDL_PauseAudio(1);
	SDL_CloseAudio();
}

void Sounds::audioPlay(const char* file) {
	int index;
	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 len;
	SDL_AudioCVT cvt;
	// find an empty audio slot
	for(index = 0; index < SOUNDS; ++index) {
		if(sounds[index].pos == sounds[index].len) 
			break;
	}
	if(index == SOUNDS)
		return;
	// load wave file and convert it to 16 Bit and 22KHz
	if(SDL_LoadWAV(file, &wave, &data, &len) == NULL) {
		fprintf(stderr, "Konnte '%s' nicht laden: %s\n", file, SDL_GetError());
		return;
	}
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_S16, 2, 22050);
	//cvt.buf = malloc(len *cvt.len_mult);
	cvt.buf = (Uint8 *)SDL_malloc(len *cvt.len_mult);
	memcpy(cvt.buf, data, len);
	cvt.len = len;
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);
	// Audiodaten in Slot schreiben und sofort mit Abspielen beginnen
	// write audio data to slot and play it
	if(sounds[index].data) 
		free(sounds[index].data);
	SDL_LockAudio();
	sounds[index].data = cvt.buf;
	sounds[index].len = cvt.len_cvt;
	sounds[index].pos = 0;
	SDL_UnlockAudio();
}

void Sounds::munch() {
	if(munch_toggle) {
		munch_toggle = false;
		this->audioPlay("/usr/local/share/pacman/sounds/munch_a.wav");
	} else {
		munch_toggle = true;
		this->audioPlay("/usr/local/share/pacman/sounds/munch_b.wav");
	}
}
		
void MyAudioCallback(void *udata, Uint8 *stream, int len) {
	unsigned int i;
	Uint32 amount;
	for(i = 0; i < SOUNDS; ++i) {
		amount = (sounds[i].len - sounds[i].pos);
		if(amount > len) 
			amount = len;
		SDL_MixAudio(stream, &sounds[i].data[sounds[i].pos], amount, SDL_MIX_MAXVOLUME);
		sounds[i].pos += amount;
	}
}