
#ifndef GAME_LAUNCHER
#define GAME_LAUNCHER

#include "math/vector2.h"
#include "engine/loop.h"
#include "bullet.h"

namespace Game {
	class Launcher {
	private:
		constexpr static f32 SHOOT_TIME = 3;

		Vector2f position;
		f32 direction;

		f32 timer;

	public:
		Launcher(Vector2f, f32);

		auto update(CNGE::Timing*) -> bool;

		auto getBullet() -> Bullet;
	};
}

#endif
