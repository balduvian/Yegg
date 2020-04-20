
#ifndef CNGE_BLOCK
#define CNGE_BLOCK

#include "../Types.h"

namespace CNGE {

	class Block {
	public:
		constexpr static bool solids[6] {
			false, true, false, true, false, false
		};

		constexpr static u8 BLOCK_GROUND = 0;
		constexpr static u8 COLOR_BLOCK_GROUND = 255;

		constexpr static u8 BLOCK_WALL = 1;
		constexpr static u8 COLOR_BLOCK_WALL = 0;

		constexpr static u8 BLOCK_SPAWN = 2;
		constexpr static u8 COLOR_BLOCK_SPAWN = 50;
		constexpr static u8 SPAWN_COLOR_INCREMENT = 10;

		constexpr static u8 BLOCK_RECIEVER = 3;
		constexpr static u8 COLOR_BLOCK_RECIEVER = 100;

		constexpr static u8 BLOCK_DOOR = 4;
		constexpr static u8 COLOR_BLOCK_DOOR = 150;

		constexpr static u8 BLOCK_END = 5;
		constexpr static u8 COLOR_BLOCK_END = 200;

	public:
		u8 id;
		u32 state;

	};

}

#endif
