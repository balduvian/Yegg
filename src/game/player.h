
#ifndef GAME_PLAYER
#define GAME_PLAYER

#include "engine/input.h"
#include "engine/loop.h"
#include "engine/camera.h"
#include "util/color.h"

#include "math/vector2.h"
#include "gameCCD.h"

namespace Game {
	class Player {
	public:
		Player(f32, f32);

		auto update(CNGE::Input*, CNGE::Timing*, LineLoop*) -> void;

		auto render(CNGE::Camera*) -> void;

	private:
		constexpr static f32 SPEED = 0.5f;
		constexpr static f32 MAX_SPEED = 6.f;
		constexpr static f32 FRICTION = 6.f;
		constexpr static f32 GRAVITY = 1;
		constexpr static f32 MAX_FALL = 6.f;

		static CNGE::Color COLOR;

		Vector2f velocity;

		Vector2f position;
		GameCCD ccd;

		bool onGround;

		std::vector<Vector2f> offsets;
	};
}

#endif
