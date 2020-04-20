
#include "block.h"

namespace Game {
	Block::Block(CNGE::Color&& displayColor, CNGE::VAO* vao, f32 rotation, i32 color, bool solid, CCDLine* right, CCDLine* up, CCDLine* left, CCDLine* down, Rotation* rightDirection, Rotation* upDirection, Rotation* leftDirection, Rotation* downDirection)
		: displayColor(displayColor), vao(vao), rotation(rotation), color(color), solid(solid), lineRight(right), lineUp(up), lineLeft(left), lineDown(down), directions() {

		directions[Rotation::right()] = rightDirection;
		directions[Rotation::up()] = upDirection;
		directions[Rotation::left()] = leftDirection;
		directions[Rotation::down()] = downDirection;
	}

	auto Block::getRightLine() -> CCDLine* {
		return lineRight;
	}

	auto Block::getUpLine() -> CCDLine* {
		return lineUp;
	}

	auto Block::getLeftLine() -> CCDLine* {
		return lineLeft;
	}

	auto Block::getDownLine() -> CCDLine* {
		return lineDown;
	}

	auto Block::getVAO() -> CNGE::VAO* {
		return vao;
	}

	auto Block::getRotation() -> f32 {
		return rotation;
	}

	auto Block::getSolid() -> bool {
		return solid;
	}

	auto Block::getDirection(i32 side) -> Rotation* {
		return directions[side];
	}

	auto Block::getDisplayColor() -> CNGE::Color& {
		return displayColor;
	}

	Block::~Block() {
		if (lineRight)
			delete lineRight;
		if (lineUp)
			delete lineUp;
		if (lineLeft)
			delete lineLeft;
		if (lineDown)
			delete lineDown;

		delete directions[Rotation::right()];
		delete directions[Rotation::up()];
		delete directions[Rotation::left()];
		delete directions[Rotation::down()];
	}
}
