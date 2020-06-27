
#include "gameCCD.h"

namespace Game {
	GameCCD::GameCCD() : CCDEngine(), loop(nullptr) {}

	auto GameCCD::setLoop(LineLoop* loop) -> void {
		this->loop = loop;
	}

	auto GameCCD::gather(CCDLine&& movement, Vector2f& offset) -> void {
		for (auto i = loop->begin(); i < loop->end(); ++i) {
			test(movement, offset, i);
		}
	}
}
