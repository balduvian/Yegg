
#ifndef GAME_LINE_LOOP
#define GAME_LINE_LOOP

#include <initializer_list>
#include "../ccd/CCDLine.h"

namespace Game {
	class LineLoop {
	private:
		i32 length;
		CCDLine* lines;

		bool connected;
	
		LineLoop(i32, bool);

		auto init(const Vector2f*, const Vector2f*) -> void;

	public:
		LineLoop(i32, Vector2f*, bool = true);
		LineLoop(const LineLoop&);
		LineLoop(LineLoop&&) noexcept;

		LineLoop(std::initializer_list<Vector2f>, bool = true);

		~LineLoop();

		auto get(i32) -> CCDLine*;

		auto begin() -> CCDLine*;

		auto end() -> CCDLine* ;

		auto getConnected() -> bool;
	};
}

#endif
