
#ifndef GAME_CCD_ENGINE
#define GAME_CCD_ENGINE

#include <memory>
#include <vector>

#include "ccdLine.h"
#include "ccdMemory.h"

namespace Game {
	struct CollStruct {
		CCDLine* line;
		Vector2f position;
		i32 side;

		CollStruct(CCDLine*, Vector2f&&, i32);
	};

	class CCDEngine {
	private:
		constexpr static u32 MEMORY_SIZE = 8;

		/* between runs */
		CCDMemory persistentMemory;

		/* during a run */
		bool foundCol;
		float closestT;
		CCDMemory tempMemory;

		/* result */
		std::unique_ptr<CollStruct> collision;

	protected:
		virtual auto gather(CCDLine&& movement, Vector2f& offset) -> void = 0;

	public:
		CCDEngine();

		auto test(CCDLine& movement, Vector2f& offset, CCDLine* wall) -> void;

		auto start(std::vector<Vector2f>&, CCDLine) -> void;

		auto getCollision() -> CollStruct*;
	};
}

#endif
