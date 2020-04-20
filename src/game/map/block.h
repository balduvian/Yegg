
#ifndef GAME_BLOCK
#define GAME_BLOCK

#include "../ccd/ccdLine.h"
#include "engine/vao.h"
#include "rotation.h"
#include "util/color.h"

namespace Game {
	class Block {
	private:
		CNGE::Color displayColor;
		CNGE::VAO* vao;
		f32 rotation;

		i32 color;

		bool solid;

		CCDLine* lineRight;
		CCDLine* lineUp;
		CCDLine* lineLeft;
		CCDLine* lineDown;
		
		friend class Blocks;

		Rotation* directions[Rotation::DIRECTIONS];

	public:
		Block(CNGE::Color&&, CNGE::VAO*, f32, i32, bool, CCDLine*, CCDLine*, CCDLine*, CCDLine*, Rotation*, Rotation*, Rotation*, Rotation*);

		auto   getRightLine() -> CCDLine*;
		auto      getUpLine() -> CCDLine*;
		auto    getLeftLine() -> CCDLine*;
		auto    getDownLine() -> CCDLine*;

		auto getVAO() -> CNGE::VAO*;
		auto getRotation() -> f32;
		auto getSolid() -> bool;

		auto getDirection(i32) -> Rotation*;

		auto getDisplayColor() -> CNGE::Color&;

		~Block();
	};
}

#endif
