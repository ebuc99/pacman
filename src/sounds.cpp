#include "sounds.h"
#include <iostream>

Sounds *Sounds::instance = NULL;

Sounds *Sounds::getInstance() {
	if (instance == NULL) {
		instance = new Sounds();
	}
	return instance;
}

void Sounds::cleanUpInstance() {
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

Sounds::Sounds():
	channel_munch(-1),
	munch_toggle(true),
	eat_ghost_cnt(0),
	enabled(true) {
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 2048; //1024;
	char filePath[256];

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		std::cerr << "Unable to initialize audio: " << Mix_GetError() << std::endl;
		exit(1);
	}
	getFilePath(filePath, "sounds/munch_a.wav");
	chunk_munch_a = Mix_LoadWAV(filePath);
	if(chunk_munch_a == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/munch_b.wav");
	chunk_munch_b = Mix_LoadWAV(filePath);
	if(chunk_munch_b == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/intro.wav");
	music_intro = Mix_LoadMUS(filePath);
	if(music_intro == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/siren_slow.wav");
	music_siren_slow = Mix_LoadMUS(filePath);
	if(music_siren_slow == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/death_1.wav");
	chunk_dying = Mix_LoadWAV(filePath);
	if(chunk_dying == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/extra_man.wav");
	chunk_extra_man = Mix_LoadWAV(filePath);
	if(chunk_extra_man == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/fruit.wav");
	chunk_fruit = Mix_LoadWAV(filePath);
	if(chunk_fruit == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/large_pellet_loop.wav");
	music_superpill_loop = Mix_LoadMUS(filePath);
	if(music_superpill_loop == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/ghost_eat_3.wav");
	chunk_eat_ghost = Mix_LoadWAV(filePath);
	if(chunk_eat_ghost == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
	getFilePath(filePath, "sounds/ghost_eat_1.wav");
	music_eat_ghost = Mix_LoadMUS(filePath);
	if(music_eat_ghost == NULL) {
		std::cerr << "Unable to load WAV file: " << Mix_GetError() << std::endl;
	}
}

Sounds::~Sounds() {
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
	Mix_FreeChunk(chunk_munch_a);
	Mix_FreeChunk(chunk_munch_b);
	Mix_FreeMusic(music_intro);
	Mix_FreeMusic(music_siren_slow);
	Mix_FreeMusic(music_superpill_loop);
	Mix_FreeMusic(music_eat_ghost);
	Mix_FreeChunk(chunk_extra_man);
	Mix_FreeChunk(chunk_dying);
	Mix_FreeChunk(chunk_fruit);
	Mix_FreeChunk(chunk_eat_ghost);
	Mix_CloseAudio();
}

bool Sounds::isEnabled() const {
	return enabled;
}

void Sounds::setEnabled(bool newValue) {
	enabled = newValue;
	if (!enabled) {
		music_stop();
		channelStop();
	}
}

void Sounds::toggleEnabled() {
	setEnabled(!isEnabled());
}

void Sounds::munch() {
	if(enabled) {
		if (channel_munch != -1) {
			Mix_HaltChannel(channel_munch);
		}
		if(munch_toggle) {
			channel_munch = Mix_PlayChannel(channel_munch, chunk_munch_a, 0);
		} else {
			channel_munch = Mix_PlayChannel(channel_munch, chunk_munch_b, 0);
		}
		if(channel_munch == -1) {
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
		}
		munch_toggle = !munch_toggle;
	}
}

void Sounds::intro() {
	if(enabled && !Mix_PlayingMusic())
		if((Mix_PlayMusic(music_intro, 1)) == -1)
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
}
void Sounds::siren_start() {
	if(enabled && !Mix_PlayingMusic())
		if((Mix_PlayMusic(music_siren_slow, -1)) == -1)
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
}
void Sounds::superpill_start() {
	if(enabled && !Mix_PlayingMusic())
		if((Mix_PlayMusic(music_superpill_loop, -1)) == -1)
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
}

void Sounds::eat_ghost_start() {
	if(eat_ghost_cnt) {
		this->music_stop();
		if(enabled && (Mix_PlayMusic(music_eat_ghost, -1)) == -1)
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
	}
}

void Sounds::eat_ghost_stop() {
	if(eat_ghost_cnt > 0) {
		if(!--eat_ghost_cnt) {
			this->music_stop();
			if(enabled)
				this->superpill_start();
		}
	}
}

void Sounds::music_stop() {
	if(Mix_PlayingMusic())
		Mix_HaltMusic();
	eat_ghost_cnt = 0;
}

void Sounds::channelStop() {
	Mix_HaltChannel(-1);
}
void Sounds::pause_all() {
	if(!Mix_PausedMusic())
		Mix_PauseMusic();
	Mix_Pause(-1);
}

void Sounds::resume_all() {
	if(enabled) {
		if(Mix_PausedMusic())
			Mix_ResumeMusic();
		Mix_Resume(-1);
	}
}

void Sounds::playSingleSound(SingleSounds singlesounds) {
	int channel;
	if(enabled) {
		if(singlesounds == DYING)
			channel = Mix_PlayChannel(-1, chunk_dying, 0);
		else if(singlesounds == EXTRA_MAN)
			channel = Mix_PlayChannel(-1, chunk_extra_man, 0);
		else if(singlesounds == FRUIT)
			channel = Mix_PlayChannel(-1, chunk_fruit, 0);
		else if(singlesounds == EAT_GHOST) {
			channel = Mix_PlayChannel(-1, chunk_eat_ghost, 0);
			++eat_ghost_cnt;
		}
		else
			return;

		if(channel == -1) {
			std::cerr << "Unable to play WAV file: " << Mix_GetError() << std::endl;
		}
	}
}
