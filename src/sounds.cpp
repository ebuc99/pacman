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
	music_intro = Mix_LoadMUS("/usr/local/share/pacman/sounds/intro.wav");
	if(music_intro == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	music_siren_slow = Mix_LoadMUS("/usr/local/share/pacman/sounds/siren_slow.wav");
	if(music_siren_slow == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_dying = Mix_LoadWAV("/usr/local/share/pacman/sounds/death_1.wav");
	if(chunk_dying == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_extra_man = Mix_LoadWAV("/usr/local/share/pacman/sounds/extra_man.wav");
	if(chunk_extra_man == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_fruit = Mix_LoadWAV("/usr/local/share/pacman/sounds/fruit.wav");
	if(chunk_fruit == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	music_superpill_loop = Mix_LoadMUS("/usr/local/share/pacman/sounds/large_pellet_loop.wav");
	if(music_superpill_loop == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
	chunk_eat_ghost = Mix_LoadWAV("/usr/local/share/pacman/sounds/ghost_eat_3.wav");
	if(chunk_eat_ghost == NULL) {
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	}
}

Sounds::~Sounds() {
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
	Mix_FreeChunk(chunk_munch_a);
	Mix_FreeChunk(chunk_munch_b);
	Mix_FreeMusic(music_intro);
	Mix_FreeMusic(music_siren_slow);
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

void Sounds::intro() {
	if(!Mix_PlayingMusic())
		if((Mix_PlayMusic(music_intro, 1)) == -1)
			fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
}
void Sounds::siren_start() {
	if(!Mix_PlayingMusic())
		if((Mix_PlayMusic(music_siren_slow, -1)) == -1)
			fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
}
	
void Sounds::music_stop() {
	if(Mix_PlayingMusic()) 
		Mix_HaltMusic();
}

void Sounds::pause_all() {
	if(!Mix_PausedMusic())
		Mix_PauseMusic();
	Mix_Pause(-1);
}

void Sounds::resume_all() {
	if(Mix_PausedMusic())
		Mix_ResumeMusic();
	Mix_Resume(-1);
}

void Sounds::playSingleSound(SingleSounds singlesounds) {
	int channel;
	if(singlesounds == DYING)
		channel = Mix_PlayChannel(-1, chunk_dying, 0);
	else if(singlesounds == EXTRA_MAN)
		channel = Mix_PlayChannel(-1, chunk_extra_man, 0);
	else if(singlesounds == FRUIT)
		channel = Mix_PlayChannel(-1, chunk_fruit, 0);
	else if(singlesounds == EAT_GHOST)
		channel = Mix_PlayChannel(-1, chunk_eat_ghost, 0);
	else
		return;
	
	if(channel == -1) {
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	}
}
