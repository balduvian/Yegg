
#ifndef CNGE_WAV_FILE
#define CNGE_WAV_FILE

#include "types.h"

namespace CNGE {
	class Wav {
	private:
		u32 format, sampleRate, size;
		u8* data;

	public:
		Wav(const char* path);
		u32 getFormat();
		u32 getSampleRate();
		u32 getSize();
		u8* getData();
		~Wav();
	};
}

#endif
