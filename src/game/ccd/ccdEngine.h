
#ifndef GAME_CCD_ENGINE
#define GAME_CCD_ENGINE

#include <memory>
#include <vector>

#include "ccdLine.h"
#include "ccdMemory.h"
#include "../map/blockMap.h"
#include "../egg/hitbox.h"
#include "../map/rotation.h"

namespace Game {
	class BlockMap;

	struct CollStruct {
		Block* block;
		Vector2f wallVector;
		Vector2f position;
		i32 side;

		CollStruct(Block*, Vector2f&, Vector2f&&, i32);
	};

	class CCDEngine {
	private:
		constexpr static u32 MEMORY_SIZE = 8;

		/* during a run */
		bool foundCol;
		float closestT;

		/* result */
		std::unique_ptr<CollStruct> collision;

	public:
		CCDEngine();

		auto start(std::vector<Hitbox>&, CCDLine, BlockMap*) -> void;

		auto gatherForHitbox(CCDLine&&, Hitbox&, BlockMap*) -> void;

		auto lineTest(Block*, CCDLine&, Vector2f&, CCDLine&) -> void;
		auto circleTest(CCDLine&, Vector2f&, Vector2f, f32) -> void;

		auto getCollision()->CollStruct*;

		i32 left, right, down, up;
	};
}

#endif
