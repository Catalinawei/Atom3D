#pragma once
#define SDL_MAIN_HANDLED	
#include <SDL.h> 
#include <memory>
#include <iostream>

namespace AtomToolKit {

	struct Sound {
		SDL_AudioSpec wav_spec;
		Uint32 wav_length;
		Uint8* wav_buffer;
	};

	class Audio {
	public:

		Audio() 
			: m_device(0) {
			if (SDL_Init(SDL_INIT_AUDIO) < 0) {
				std::cout << "Cannot init sdl \n";
				return;
			}

			auto desired = std::make_unique<SDL_AudioSpec>();
			auto obtained = std::make_unique<SDL_AudioSpec>();

			desired->freq = 44100; //Hertz, as always
			desired->format = AUDIO_S32LSB;//32 bit
			desired->samples = 8192; //idk
			desired->userdata = NULL;
			desired->channels = 1; //mono

			m_device = SDL_OpenAudioDevice(nullptr, 0, desired.get(), obtained.get(), 0);
		}

		bool loadWavFile(const char* file, Sound& sound){
			if (!SDL_LoadWAV(file, &sound.wav_spec, &sound.wav_buffer, &sound.wav_length)) {
				std::cerr << "Couldnt load file " << file << "\n";
				return false;
			}

			return true;
		}

		void PlaySimpleAudio(const Sound& sound) const {
			if (SDL_QueueAudio(m_device, sound.wav_buffer, sound.wav_length) < 0) {
				std::cout << "CANNOT QUEUE Audio " << SDL_GetError() << "\n";
				return;
			}
			SDL_PauseAudioDevice(m_device, 0);
		}

		void ChangeVolume(const Sound& sound, SDL_AudioFormat format, int volume) const {
			SDL_MixAudioFormat(sound.wav_buffer, sound.wav_buffer, format, sound.wav_length, volume);
		}
	private:
		SDL_AudioDeviceID m_device;
	};
}