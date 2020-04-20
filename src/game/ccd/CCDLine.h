
#ifndef GAME_CCD_LINE
#define GAME_CCD_LINE

#include "math/vector2.h"
#include <iostream>

namespace Game {
	class CCDLine {
	public:
		CCDLine();

		CCDLine(f32, f32, f32, f32);

		CCDLine(const Vector2f&, const Vector2f&&);
		CCDLine(const Vector2f&&, const Vector2f&&);
		CCDLine(const Vector2f*, const Vector2f*);
		CCDLine(const CCDLine*, i32, i32);

		friend auto operator<<(std::ostream&, const CCDLine&) -> std::ostream&;

		Vector2f start;
		Vector2f end;

		auto set(f32, f32, f32, f32) -> void;
		auto set(const Vector2f*, const Vector2f*) -> void;

		auto toVector() -> Vector2f;

		auto operator+(const Vector2f&) -> CCDLine;
		auto operator-(const Vector2f&)->CCDLine;
		auto operator*(f32)->CCDLine;

		auto setEnd(const Vector2f&) -> void;
	};
}

#endif
