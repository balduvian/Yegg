
#ifndef CNGE_AUDIO_SOURCE
#define CNGE_AUDIO_SOURCE

#include "load/resource.h"

#include "types.h"
#include "audioBuffer.h"

namespace CNGE {
	class Sound : public Resource {
	private:
		u32 source;

		const char* wavFilePath;
		std::unique_ptr<Wav> wav;

	public:
		Sound(const char*);
		
		void play();
		void pause();
		void stop();
		
		void loop(bool = true);
		
		auto customGather() -> void override;
		auto customProcess() -> void override;
		auto customDiscard() -> void override;
		auto customUnload() -> void override;

		auto setVolume(f32);

		~Sound(); 
	};
}

#endif
