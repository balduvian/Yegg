
#ifndef GAME_EGG_SHELL
#define GAME_EGG_SHELL

#include "math/vector2.h"
#include "engine/loop.h"
#include "engine/camera.h"
#include <random>

namespace Game {
	class Eggshell {
	private:
		constexpr static f32 WIDTH = 16.f;
		constexpr static f32 HEIGHT = 16.f;

		static std::default_random_engine engine;

		static std::uniform_real_distribution<f32> rotationDistribution;
		static std::uniform_real_distribution<f32> speedDistribution;

		Vector2f position;
		Vector2f velocity;
		f32 rotation;

	public:
		Eggshell(Vector2f position);

		auto update(CNGE::Timing* timing) -> void;

		auto render(CNGE::Camera* camera) -> void;
	};
}

#endif
