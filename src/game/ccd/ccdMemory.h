
#ifndef GAME_CCD_MEMORY
#define GAME_CCD_MEMORY

#include "CCDLine.h"

namespace Game {
	struct MemoryStruct {
		CCDLine* line;
		i8 side;

		MemoryStruct();
		MemoryStruct(CCDLine*, i8);
	};

	class CCDMemory {
	public:
		CCDMemory(i32);
		~CCDMemory();

		auto add(CCDLine*, i8) -> void;

		auto contains(CCDLine*) -> bool;

		auto get(CCDLine*) -> MemoryStruct*;

		auto empty() -> void;

		auto begin() -> MemoryStruct*;

		auto end() -> MemoryStruct*;

	private:
		i32 size;
		i32 start;

		MemoryStruct* array;
	};
}

#endif
