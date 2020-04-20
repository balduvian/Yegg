
#include "rotation.h"
#include "math/math.h"

namespace Game {
	Rotation::Rotation() : rotation(0) {}

	Rotation::Rotation(i32 rotation) : rotation(rotation) {}

	auto Rotation::operator-(const Rotation& other) -> Rotation {
		auto newRotation = rotation - other.rotation;

		newRotation = CNGE::mod(newRotation, DIRECTIONS);

		return { newRotation };
	}

	auto Rotation::operator-=(const Rotation& other) -> Rotation& {
		rotation -= other.rotation;

		rotation = CNGE::mod(rotation, DIRECTIONS);

		return *this;
	}

	auto Rotation::operator+(const Rotation& other) -> Rotation {
		auto newRotation = rotation + other.rotation;

		newRotation = CNGE::mod(newRotation, DIRECTIONS);

		return { newRotation };
	}

	auto Rotation::operator==(const Rotation& other) -> bool {
		return rotation == other.rotation;
	}

	auto Rotation::operator!=(const Rotation& other) -> bool {
		return rotation != other.rotation;
	}

	Rotation::operator i32() {
		return rotation;
	}

	auto Rotation::rotate(i32 amount) -> void {
		rotation += amount;

		rotation = CNGE::mod(rotation, DIRECTIONS);
	}

	auto Rotation::rotate(const Rotation& rotation, i32 amount) -> Rotation {
		auto newRotation = rotation.rotation + amount;

		newRotation = CNGE::mod(newRotation, DIRECTIONS);

		return { newRotation };
	}

	auto Rotation::set(i32 amount) -> void {
		rotation = amount;
	}

	auto Rotation::getX() -> i32 {
		return X_TABLE[rotation];
	}

	auto Rotation::getY() -> i32 {
		return Y_TABLE[rotation];
	}

	auto Rotation::right() -> Rotation {
		return Rotation(0);
	}

	auto Rotation::upRight() -> Rotation {
		return Rotation(1);
	}

	auto Rotation::up() -> Rotation {
		return Rotation(2);
	}

	auto Rotation::upLeft() -> Rotation {
		return Rotation(3);
	}

	auto Rotation::left() -> Rotation {
		return Rotation(4);
	}

	auto Rotation::downLeft() -> Rotation {
		return Rotation(5);
	}

	auto Rotation::down() -> Rotation {
		return Rotation(6);
	}

	auto Rotation::downRight() -> Rotation {
		return Rotation(7);
	}

	auto Rotation::is(Rotation* ptr, Rotation compare) -> bool {
		if (ptr)
			return (*ptr == compare);

		return false;
	}
}