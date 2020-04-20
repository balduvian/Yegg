#pragma once

#ifndef GAME_HITBOX
#define GAME_HITBOX

#include "math/vector2.h"

namespace Game {
	class Hitbox {
	private:
		Vector2f position;
		f32 radius;

	public:
		Hitbox(Vector2f&&, f32);

		Hitbox(Hitbox&, Vector2f&&, f32);

		auto getPosition() -> Vector2f&;
		auto getRadius() -> f32;
	};
}

#endif
