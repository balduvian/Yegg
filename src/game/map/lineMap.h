
#ifndef GAME_LINE_MAP
#define GAME_LINE_MAP

#include <vector>

#include "lineLoop.h"

namespace Game {
	class LineMap {
	private:
		i32 numLineLoops;
		LineLoop** lineLoops;

		int sectionsWide;
		int sectionsTall;
		LineLoop*** sections;

	public:
		LineMap(i32, LineLoop*);

		LineLoop** getLineLoops(int, int);

	};
}

#endif
