
#ifndef GAME_CCD_LINE
#define GAME_CCD_LINE

#include "math/vector2.h"

namespace Game {
	class CCDLine {
	public:
		CCDLine();

		CCDLine(f32, f32, f32, f32);
		CCDLine(f32, f32, f32, f32, const char*);
		CCDLine(const Vector2f&, const Vector2f&&);
		CCDLine(const Vector2f&&, const Vector2f&&);
		CCDLine(const Vector2f*, const Vector2f*);

		Vector2f start;
		Vector2f end;

		//DEBUG
		const char* name;

		auto set(f32, f32, f32, f32) -> void;
		auto set(const Vector2f*, const Vector2f*) -> void;

		auto toVector() -> Vector2f;

		auto operator+(const Vector2f&) -> CCDLine;

		auto setEnd(const Vector2f&) -> void;
	};
}

#endif
