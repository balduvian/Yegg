
#include "launcher.h"

namespace Game {
	Launcher::Launcher(Vector2f position, f32 direction)
		: position(position), direction(direction), timer(SHOOT_TIME) {};

	auto Launcher::update(CNGE::Timing* timing) -> bool {
		timer -= timing->time;

		if (timer <= 0) {
			timer = SHOOT_TIME - timer;

			return true;
		}

		return false;
	}

	auto Launcher::getBullet() -> Bullet {
		return Bullet(position, direction);
	}
}
