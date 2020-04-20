
#ifndef GAME_EGG
#define GAME_EGG

#include "engine/window.h"
#include "engine/loop.h"
#include "engine/camera.h"
#include "hitbox.h"
#include "util/color.h"
#include "../ccd/ccdEngine.h"
#include "math/vector2.h"
#include "util/aspect.h"

namespace Game {
	class Egg {
	private:
		constexpr static f32 BOX_WIDTH = 22.f;
		constexpr static f32 BOX_HEIGHT = 29.f;

		constexpr static f32 GRAVITY = 6.f;
		constexpr static f32 SPEED = 3.f;

		constexpr static f32 SHOOTING_SPEED = 32.f;

		Vector2f position;
		Vector2f velocity;

		f32 rotation;

		std::vector<Hitbox> anchorHitbox;

		Vector2f anchorPosition;
		Vector2f shootingAngle;
		bool anchored;
		bool shooting;
		f32 anchorLength;

		bool shouldDie;
		bool shouldWin;

		auto renderHitBox(CNGE::Camera*, CNGE::Color&, Hitbox&) -> void;

	public:

		std::vector<Hitbox> hitboxes;
		CCDEngine ccd;

		static CNGE::Color color0;
		static CNGE::Color color1;
		static CNGE::Color color2;

		Egg(Vector2f);

		auto update(CNGE::Input*, CNGE::Timing*, BlockMap*, CNGE::Aspect*, CNGE::Camera*) -> void;

		auto render(CNGE::Camera*) -> void;

		auto setAnchor(Vector2f) -> void;

		auto getPosition() -> Vector2f;

		auto getShouldDie() -> bool;
		auto getShouldWin() -> bool;
	};
}

#endif
