
#include <limits>

#include "ccdEngine.h"
#include "math/math.h"
#include <iostream>
#include "../map/blocks.h"

namespace Game {
	CollStruct::CollStruct(Block* block, Vector2f& wallVector, Vector2f&& position, i32 side)
		: block(block), wallVector(wallVector), position(position), side(side) {}

	CCDEngine::CCDEngine()
		: foundCol(), closestT(FLT_MAX), collision() {}

	auto CCDEngine::start(std::vector<Hitbox>& hitboxes, CCDLine movement, BlockMap* map) -> void {
		foundCol = false;
		collision = nullptr;

		do {
			foundCol = false;
			closestT = FLT_MAX;

			for (auto& hitbox : hitboxes)
				gatherForHitbox(movement + hitbox.getPosition(), hitbox, map);

			if (foundCol)
				movement.setEnd(collision->position);

		} while (foundCol);
	}

	/* called once per hitbox */
	auto CCDEngine::gatherForHitbox(CCDLine&& offsetMovement, Hitbox& hitbox, BlockMap* map) -> void {
		auto movingDown = offsetMovement.end.y() < offsetMovement.start.y();
		auto movingUp = offsetMovement.end.y() > offsetMovement.start.y();

		auto movingLeft = offsetMovement.end.x() < offsetMovement.start.x();
		auto movingRight = offsetMovement.end.x() > offsetMovement.start.x();

		auto x0 = offsetMovement.start.x();
		auto x1 = offsetMovement.end.x();
		auto y0 = offsetMovement.start.y();
		auto y1 = offsetMovement.end.y();
		
		CNGE::minMax(x0, x1);
		CNGE::minMax(y0, y1);

		x0 -= hitbox.getRadius();
		y0 -= hitbox.getRadius();
		x1 += hitbox.getRadius();
		y1 += hitbox.getRadius();

		auto pos0 = map->worldToMap(x0, y0);
		auto pos1 = map->worldToMap(x1, y1);

		left = pos0.x - 1;
		down = pos0.y - 1;
		right = pos1.x + 1;
		up = pos1.y + 1;

		//std::cout << left << " | " << down << " | " << right << " | " << up << std::endl;

		for (auto j = down; j <= up; ++j) {
			for (auto i = left; i <= right; ++i) {
				auto current = map->getBlock(i, j);
				/* only collide with solid blocks */
				if (current->getSolid()) {

					auto up = Rotation::up();
					auto down = Rotation::down();
					auto left = Rotation::left();
					auto right = Rotation::right();

					auto doTest = [this, current, &offsetMovement, map, &hitbox, i, j](CCDLine* wall) {
						if (wall) {
							auto wallPush = wall->toVector().rotate(CNGE::PI / 2).setLength(hitbox.getRadius());

							/* wall is tied to block and normalized */
							/* so it must be scaled by the size of block */
							/* and moved to where the block is */
							auto offsetWall = ((*wall * BlockMap::TILE_SIZE) + Vector2f(i * BlockMap::TILE_SIZE, j * BlockMap::TILE_SIZE)) + wallPush;

							lineTest(current, offsetMovement, hitbox.getPosition(), offsetWall);
						}
					};

					/* the direciton of the facing block's wall can't be the flat for that face side */

					if (movingDown && !Rotation::is(map->getBlock(i + up.getX(), j + up.getY())->getDirection(down), left)) {
						auto wall = current->getUpLine();
						doTest(wall);
					}

					if (movingLeft && !Rotation::is(map->getBlock(i + right.getX(), j + right.getY())->getDirection(left), up)) {
						auto wall = current->getRightLine();
						doTest(wall);
					}

					if (movingUp && !Rotation::is(map->getBlock(i + down.getX(), j + down.getY())->getDirection(up), right)) {
						auto wall = current->getDownLine();
						doTest(wall);
					}

					if (movingRight && !Rotation::is(map->getBlock(i + left.getX(), j + left.getY())->getDirection(right), down)) {
						auto wall = current->getLeftLine();
						doTest(wall);
					}
				}

				if (map->getCircle(i, j)) {
					auto circlePos = Vector2f(i * BlockMap::TILE_SIZE, j * BlockMap::TILE_SIZE);
					circleTest(offsetMovement, hitbox.getPosition(), circlePos, hitbox.getRadius());
				}
			}
		}
	}

	auto CCDEngine::lineTest(Block* block, CCDLine& movement, Vector2f& offset, CCDLine& offsetWall) -> void {
		auto wallVector = offsetWall.toVector();

		auto wallAngle = wallVector.getAngle();

		auto wallWidth = wallVector.getLength();

		auto relativeStart = (movement.start - offsetWall.start).rotate(-wallAngle);
		auto   relativeEnd = (movement.end - offsetWall.start).rotate(-wallAngle);

		auto moveT = CNGE::invInterp(relativeStart.y(), relativeEnd.y(), 0.f);

		/* only collide at the closest wall */
		if (CNGE::inclusiveRange(0.f, moveT, 1.f) && moveT < closestT) {
			/* test if movement intersects with wall */
			auto collisionX = CNGE::interp(relativeStart.x(), relativeEnd.x(), moveT);

			auto intersection = CNGE::inclusiveRange(0.f, collisionX, wallWidth);

			/* if movement intersects with wall */
			if (intersection) {
				auto wallPush = rotate(wallVector, (CNGE::PI / 2)).setLength(0.01f);

				auto finalPosition = project((movement.end - offsetWall.start), wallVector) + offsetWall.start;

				collision = std::make_unique<CollStruct>(block, wallVector, finalPosition - offset + wallPush, CNGE::sign(relativeStart.y()));

				foundCol = true;

				closestT = moveT;
			}
		}
	}

	auto CCDEngine::circleTest(CCDLine& movement, Vector2f& offset, Vector2f circlePos, f32 radius) -> void {
		auto offsetMovement = movement - circlePos;

		auto x0 = offsetMovement.start.x();
		auto x1 = offsetMovement.end.x();
		auto y0 = offsetMovement.start.y();
		auto y1 = offsetMovement.end.y();

		/* some magic from blood gates */

		auto c = x0 * x0 + y0 * y0 - radius * radius;
		auto b = 2 * (x1 * x0 - x0 * x0 + y1 * y0 - y0 * y0);
		auto a = x1 * x1 - 2 * x1 * x0 + x0 * x0 + y1 * y1 - 2 * y1 * y0 + y0 * y0;

		auto moveT = (f32)((-b - sqrt(b * b - 4 * a * c)) / (2 * a));

		if (CNGE::inclusiveRange(0.f, moveT, 1.f) && moveT < closestT) {
			auto wallPush = movement.toVector().negate().setLength(5.f);

			auto finalPosition = CNGE::interp(movement.start, movement.end, moveT);

			auto tangent = (finalPosition - circlePos).rotate(CNGE::PI / 2);

			collision = std::make_unique<CollStruct>(&Blocks::fullBlock, tangent, finalPosition - offset + wallPush, 1);

			foundCol = true;

			closestT = moveT;
		}
	}

	auto CCDEngine::getCollision() -> CollStruct* {
		return collision.get();
	}
}
