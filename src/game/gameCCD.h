
#ifndef GAME_GAME_CCD
#define GAME_GAME_CCD

#include "ccd/ccdEngine.h"
#include "map/lineLoop.h"

namespace Game {
	class GameCCD : public CCDEngine {
	public:
		GameCCD();

		auto setLoop(LineLoop*) -> void;

	private:
		LineLoop* loop;

		auto gather(CCDLine&& movement, Vector2f& offset) -> void override;
	};
}

#endif
