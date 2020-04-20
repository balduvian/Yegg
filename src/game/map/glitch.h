
#ifndef GAME_GLITCH
#define GAME_GLITCH

#include <random>

#include "types.h"
#include "math/vector2.h"
#include "engine/loop.h"

namespace Game {
	class Glitch {
	private:
		constexpr static f32 MAX_RANGE = 32 / 2;
		constexpr static f32 MAX_TIMER = 1.f;
		constexpr static f32 MAX_SPEED = 3.f;

		static std::default_random_engine engine;

		static std::uniform_real_distribution<f32> rotationDistribution;
		static std::uniform_real_distribution<f32> distanceDistribution;
		static std::uniform_real_distribution<f32>    timerDistribution;
		static std::uniform_real_distribution<f32>    speedDistribution;

		static std::uniform_int_distribution<i32>    boolDistribution;
	
		f32 rotation;
		f32 distance;
		f32 timer;
		f32 speed;

	public:
		Glitch();

		auto reset() -> void;

		auto update(CNGE::Timing* timing) -> void;

		auto getOffset() -> Vector2f;
	};
}

#endif
