
#ifndef GAME_BLOCKS
#define GAME_BLOCKS

#include "block.h"

namespace Game {
	class Blocks {
	public:
		static Block airBlock;
		static Block spawnBlock;
		static Block goodBlock;
		static Block fullBlock;
		static Block upRightRamp;
		static Block upLeftRamp;
		static Block downRightRamp;
		static Block downLeftRamp;
		static Block rightLauncher;
		static Block upLauncher;
		static Block leftLauncher;
		static Block downLauncher;

		constexpr static i32 NUM_BLOCKS = 12;

		static Block* blockList[NUM_BLOCKS];

		static auto getBlock(i32) -> Block*;

		static auto genRightLine() -> CCDLine*;
		static auto genUpLine() -> CCDLine*;
		static auto genLeftLine() -> CCDLine*;
		static auto genDownLine() -> CCDLine*;
	};
}

#endif
