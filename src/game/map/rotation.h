
#ifndef GAME_ROTATION
#define GAME_ROTATION

#include "types.h"

namespace Game {
	class Rotation {
	public:
		constexpr static i32 DIRECTIONS = 8;

	private:
		Rotation(i32);

		constexpr static i32 X_TABLE[DIRECTIONS] = {
			1, 1, 0, -1, -1, -1, 0, 1
		};

		constexpr static i32 Y_TABLE[DIRECTIONS] = {
			0, 1, 1, 1, 0, -1, -1, -1
		};
		
		i32 rotation;

	public:
		Rotation();
		
		constexpr static i32 HALF = 4;
		constexpr static i32 QUARTER = 2;
		constexpr static i32 EIGHTH = 1;

		auto operator-(const Rotation&) -> Rotation;
		auto operator-=(const Rotation&) -> Rotation&;
		auto operator+(const Rotation&) -> Rotation;
		auto operator==(const Rotation&) -> bool;
		auto operator!=(const Rotation&) -> bool;
		operator i32();

		auto rotate(i32) -> void;
		static auto rotate(const Rotation&, i32) -> Rotation;

		static auto is(Rotation*, Rotation) -> bool;

		auto set(i32) -> void;

		auto getX() -> i32;
		auto getY() -> i32;

		static auto     right() -> Rotation;
		static auto   upRight() -> Rotation;
		static auto        up() -> Rotation;
		static auto    upLeft() -> Rotation;
		static auto      left() -> Rotation;
		static auto  downLeft() -> Rotation;
		static auto      down() -> Rotation;
		static auto downRight() -> Rotation;
	};
}

#endif
