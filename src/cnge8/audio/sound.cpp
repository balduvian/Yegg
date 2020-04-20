
#include <iostream>

#include <AL\al.h>

#include "sound.h"

namespace CNGE {
	Sound::Sound(const char* wavFilePath)
		: Resource(true), wavFilePath(wavFilePath), wav(), source() {}

	auto Sound::customGather() -> void {
		wav = std::make_unique<Wav>(wavFilePath);
	}

	auto Sound::customProcess() -> void {
		auto buffer = AudioBuffer(wav.get());

		alGenSources(1, &source);
		alSourcef(source, AL_GAIN, 1);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_DIRECTION, 0, 0, 0);
		alSourcei(source, AL_BUFFER, buffer.getBuffer());
		/* buffer dies */
	}

	auto Sound::customUnload() -> void {
		alDeleteSources(1, &source);
	}

	auto Sound::customDiscard() -> void {
		wav = nullptr;
	}

	void Sound::play() {
		alSourcePlay(source);
	}

	void Sound::stop() {
		alSourceStop(source);
	}

	void Sound::pause() {
		alSourcePause(source);
	}

	void Sound::loop(bool loop) {
		alSourcei(source, AL_LOOPING, loop);
	}

	auto Sound::setVolume(f32 volume) {
		alSourcef(source, AL_GAIN, volume);
	}

	Sound::~Sound() {
		smartDestroy();
	}
}
