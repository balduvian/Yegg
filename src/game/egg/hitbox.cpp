
#include "hitbox.h"

namespace Game {
	Hitbox::Hitbox(Vector2f&& position, f32 radius)
		: position(position), radius(radius) {}

	Hitbox::Hitbox(Hitbox& other, Vector2f&& position, f32 rotation)
		: position(rotate(other.position, rotation) + position), radius(other.radius) {}

	auto Hitbox::getPosition() -> Vector2f& {
		return position;
	}

	auto Hitbox::getRadius() -> f32 {
		return radius;
	}
}
