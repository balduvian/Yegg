
#ifndef GAME_BULLET
#define GAME_BULLET

#include "../ccd/ccdEngine.h"

namespace Game {
	class CCDEngine;
	class Hitbox;
	class BlockMap;
	class Glitch;

	class Bullet {
	public:
		constexpr static f32 SPEED = 128.f;
		constexpr static f32 SIZE = 16.f;

	private:
		static std::vector<Hitbox> hitbox;
		static CCDEngine ccd;

		f32 direction;

	public:
		Vector2f position;

		Bullet(Vector2f, f32);
		Bullet(const Bullet&);

		auto update(CNGE::Timing*, BlockMap*) -> bool;

		auto render(CNGE::Camera*, Glitch*, Glitch*, Glitch*) -> void;

		auto renderCircle(CNGE::Camera*, Glitch*, CNGE::Color*) -> void;
	};
}

#endif
