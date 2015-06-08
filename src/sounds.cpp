#include "sounds.h"

Sounds::Sounds():
	munch_toggle(true){
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 1024;
	 
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	chunk_munch_a = Mix_LoadWAV("/usr/local/share/pacman/sounds/munch_a.wav");
	if(chunk_munch_a == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_munch_b = Mix_LoadWAV("/usr/local/share/pacman/sounds/munch_b.wav");
	if(chunk_munch_a == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}	
	chunk_intro = Mix_LoadWAV("/usr/local/share/pacman/sounds/intro.wav");
	if(chunk_intro == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_siren_slow = Mix_LoadMUS("/usr/local/share/pacman/sounds/siren_slow.wav");
	if(chunk_siren_slow == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_dying = Mix_LoadWAV("/usr/local/share/pacman/sounds/death_1.wav");
	if(chunk_dying == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
}

Sounds::~Sounds() {
	Mix_HaltChannel(-1);
	Mix_FreeChunk(chunk_munch_a);
	Mix_FreeChunk(chunk_munch_b);
	Mix_FreeChunk(chunk_intro);
	Mix_FreeMusic(chunk_siren_slow);
	Mix_FreeChunk(chunk_dying);
	Mix_CloseAudio();
}

void Sounds::munch() {
	Mix_HaltChannel(channel_munch);
	if(munch_toggle) {
		munch_toggle = false;
		channel_munch = Mix_PlayChannel(-1, chunk_munch_a, 0);
	} else {
		munch_toggle = true;
		channel_munch = Mix_PlayChannel(-1, chunk_munch_b, 0);
	}
		
	if(channel_munch == -1) {
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}
}

void Sounds::siren_start() {
	if(!Mix_PlayingMusic())
		if((Mix_PlayMusic(chunk_siren_slow, -1)) == -1)
			fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
}
	
void Sounds::siren_stop() {
	if(Mix_PlayingMusic()) 
		Mix_HaltMusic();
}

void Sounds::pause_all() {
	Mix_Pause(-1);
	Mix_PauseMusic();
}

void Sounds::resume_all() {
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void Sounds::playSingleSound(SingleSounds singlesounds) {
	int channel;
	if(singlesounds == INTRO)
		channel = Mix_PlayChannel(-1, chunk_intro, 0);
	else if(singlesounds == DYING)
		channel = Mix_PlayChannel(-1, chunk_dying, 0);
	else
		return;
	
	if(channel == -1) {
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}
}
