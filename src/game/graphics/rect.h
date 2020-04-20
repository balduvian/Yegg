
#ifndef GAME_RECT
#define GAME_RECT

#include "../../cnge8/engine/vao.h"

namespace Game {
	/// a texture enabled rect with
	/// a 0,0 in the bottom left corner
	class Rect : public CNGE::VAO {
	public:
		Rect();
	};

	/// a texture enabled rect with 
	/// a 0,0 in the bottom center
	class CardRect : public CNGE::VAO {
	public:
		CardRect();
	};

	class EggShape : public CNGE::VAO {
	public:
		EggShape();
	};

	class EggshellShape : public CNGE::VAO {
	public:
		EggshellShape();
	};

	class Triangle : public CNGE::VAO {
	public:
		Triangle();
	};

	class LauncherShape : public CNGE::VAO {
	public:
		LauncherShape();
	};
}

#endif
