
#include "ccdMemory.h"

namespace Game {
	MemoryStruct::MemoryStruct() : line(nullptr), side(0) {}

	MemoryStruct::MemoryStruct(CCDLine* line, i8 side)
		: line(line), side(side) {}

	CCDMemory::CCDMemory(i32 size)
		: size(size), start(0), array(new MemoryStruct[size]) {}

	CCDMemory::~CCDMemory() {
		delete[] array;
	}

	auto CCDMemory::add(CCDLine* line, i8 side) -> void {
		if (!contains(line)) {
			array[start] = MemoryStruct(line, side);
			++start %= size;
		}
	}

	auto CCDMemory::contains(CCDLine* line) -> bool {
		for (auto i = 0; i < size; ++i)
			if (array[i].line == line)
				return true;

		return false;
	}

	auto CCDMemory::get(CCDLine* line) -> MemoryStruct* {
		for (auto i = 0; i < size; ++i)
			if (array[i].line == line)
				return (array + i);

		return nullptr;
	}

	auto CCDMemory::empty() -> void {
		for (auto i = 0; i < size; ++i)
			array[i].line = nullptr;
	}

	auto CCDMemory::begin() -> MemoryStruct* {
		return array;
	}

	auto CCDMemory::end() -> MemoryStruct* {
		return array + size;
	}
}
