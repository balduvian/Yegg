
#include <limits>

#include "ccdEngine.h"
#include "math/math.h"

#include <iostream>

namespace Game {
	CollStruct::CollStruct(CCDLine* line, Vector2f&& position, i32 side)
		: line(line), position(position), side(side) {}

	CCDEngine::CCDEngine()
		: persistentMemory(MEMORY_SIZE), tempMemory(MEMORY_SIZE), foundCol(), closestT(), collision() {}

	auto CCDEngine::start(std::vector<Vector2f>& offsets, CCDLine movement) -> void {
		/* exit condition for collision loop */
		foundCol = false;
		tempMemory.empty();
		
		collision = nullptr;

		/* "recursive" collisions until no collision is found
		 * each subsequent collision's end point is the end point
		 * of the previous collision */
		do {
			foundCol = false;
			closestT = FLT_MAX;

			/* test will be called in custom line gather */
			for (auto& offset : offsets)
				gather(movement + offset, offset);

			/* set end for new movement line */
			if (foundCol) {
				movement.setEnd(collision->position);
				tempMemory.add(collision->line, collision->side);
			}
		} while (foundCol);

		/* remember all our collisions */
		if (collision) {
			for (auto& memory : tempMemory)
				persistentMemory.add(memory.line, memory.side);
		}

		std::cout << "MEMORY: " << std::endl;
		for (auto& memory : persistentMemory) {
			if (memory.line)
				std::cout << memory.line->name << ", ";
		}
		std::cout << std::endl;
	}

	auto CCDEngine::test(CCDLine& movement, Vector2f& offset, CCDLine* wall) -> void {
		if (!tempMemory.contains(wall)) {
			auto wallVector = wall->toVector();

			auto wallAngle = wallVector.getAngle();
			auto wallWidth = wallVector.getLength();

			auto relativeStart = (movement.start - wall->start).rotate(-wallAngle);
			auto   relativeEnd = (movement.end - wall->start).rotate(-wallAngle);

			auto moveT = CNGE::invInterp(relativeStart.y(), relativeEnd.y(), 0.f);

			/* only collide at the closest wall */
			if (moveT < closestT) {
				/* test if movement intersects with wall */
				auto collisionX = CNGE::interp(relativeStart.x(), relativeEnd.x(), moveT);

				auto intersection = false;

				auto wallOverride = persistentMemory.get(wall);

				if (wallOverride)
					/* counts as a collision if your line ends on the opposite side in which you came to the wall */
					intersection = CNGE::inclusiveRange(0.f, collisionX, wallWidth) && (CNGE::sign(relativeEnd.y()) != wallOverride->side) && (CNGE::inclusiveRange(0.f, relativeStart.x(), wallWidth) || CNGE::inclusiveRange(0.f, relativeEnd.x(), wallWidth));
				else
					intersection = CNGE::inclusiveRange(0.f, collisionX, wallWidth) && CNGE::inclusiveRange(0.f, moveT, 1.f);

				/* if movement intersects with wall */
				if (intersection) {
					auto finalPosition = project((movement.end - wall->start), wallVector) + wall->start;

					if (wallOverride)
						collision = std::make_unique<CollStruct>(wall, finalPosition - offset, wallOverride->side);
					else
						collision = std::make_unique<CollStruct>(wall, finalPosition - offset, CNGE::sign(relativeStart.y()));

					foundCol = true;
				}
			}
		}
	}

	auto CCDEngine::getCollision() -> CollStruct* {
		return collision.get();
	}
}
