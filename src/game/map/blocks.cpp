
#include "blocks.h"
#include "../res.h"
#include "math/math.h"

namespace Game {
	Block Blocks::     airBlock = Block(CNGE::Color(0, 0, 0), nullptr,             0, 0xffffff, false,        nullptr,     nullptr,       nullptr,  nullptr,     nullptr, nullptr, nullptr, nullptr);
	Block Blocks::   spawnBlock = Block(CNGE::Color(0, 0, 0), nullptr, 0, 0x00ff00, false, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	Block Blocks::    goodBlock = Block(CNGE::Color(127, 127, 127), &Res::rect, 0, 0x7f7f7f, true, genRightLine(), genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::    fullBlock = Block(CNGE::Color(0, 0, 0), &Res::    rect,             0, 0x000000, true, genRightLine(),   genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::  upRightRamp = Block(CNGE::Color(0, 0, 0), &Res::triangle,             0, 0xff0000, true, genRightLine(),   genUpLine(), new CCDLine{ 1, 0, 0, 1 }, new CCDLine{ 1, 0, 0, 1 }, new Rotation(Rotation::down()), new Rotation(Rotation::right()),                      nullptr, new Rotation(Rotation::upLeft()));
	Block Blocks::   upLeftRamp = Block(CNGE::Color(0, 0, 0), &Res::triangle,  CNGE::PI / 2, 0xffff00, true, new CCDLine{ 1, 1, 0, 0 },   genUpLine(), genLeftLine(), new CCDLine{ 1, 1, 0, 0 }, new Rotation(Rotation::downLeft()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), nullptr);
	Block Blocks::downRightRamp = Block(CNGE::Color(0, 0, 0), &Res::triangle, -CNGE::PI / 2, 0xff00ff, true, genRightLine(), new CCDLine{ 0, 0, 1, 1 }, new CCDLine{ 0, 0, 1, 1 }, genDownLine(), new Rotation(Rotation::down()), nullptr, new Rotation(Rotation::upRight()), new Rotation(Rotation::left()));
	Block Blocks:: downLeftRamp = Block(CNGE::Color(0, 0, 0), &Res::triangle,  CNGE::PI    , 0x0000ff, true, new CCDLine{ 0, 1, 1, 0 }, new CCDLine{ 0, 1, 1, 0 }, genLeftLine(), genDownLine(), nullptr, new Rotation(Rotation::downRight()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::rightLauncher = Block(CNGE::Color(0, 0, 0), &Res::launcherShape, 0, 0x7f0000, true, genRightLine(), genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::upLauncher = Block(CNGE::Color(0, 0, 0), &Res::launcherShape, CNGE::PI / 2, 0x007f00, true, genRightLine(), genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::leftLauncher = Block(CNGE::Color(0, 0, 0), &Res::launcherShape, CNGE::PI, 0x00007f, true, genRightLine(), genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));
	Block Blocks::downLauncher = Block(CNGE::Color(0, 0, 0), &Res::launcherShape, CNGE::PI * 3 / 2, 0x7f007f, true, genRightLine(), genUpLine(), genLeftLine(), genDownLine(), new Rotation(Rotation::down()), new Rotation(Rotation::right()), new Rotation(Rotation::up()), new Rotation(Rotation::left()));

	Block* Blocks::blockList[NUM_BLOCKS] = {
		&airBlock,
		&spawnBlock,
		&goodBlock,
		&fullBlock,
		&upRightRamp,
		&upLeftRamp,
		&downRightRamp,
		&downLeftRamp,
		&rightLauncher,
		&upLauncher,
		&leftLauncher,
		&downLauncher
	};

	auto Blocks::getBlock(i32 color) -> Block* {
		for (auto i = 0; i < NUM_BLOCKS; ++i)
			if (blockList[i]->color == color)
				return blockList[i];

		return &airBlock;
	}

	/* all lines have their face to the left */

	auto Blocks::genRightLine() -> CCDLine* {
		return new CCDLine{ 1, 1, 1, 0 };
	}

	auto Blocks::genUpLine() -> CCDLine* {
		return new CCDLine{ 0, 1, 1, 1 };
	}

	auto Blocks::genLeftLine() -> CCDLine* {
		return new CCDLine{ 0, 0, 0, 1 };
	}

	auto Blocks::genDownLine() -> CCDLine* {
		return new CCDLine{ 1, 0, 0, 0 };
	}
}
