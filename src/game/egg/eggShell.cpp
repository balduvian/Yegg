
#include "eggshell.h"
#include "math/math.h"
#include "../res.h"

namespace Game {
	std::default_random_engine Eggshell::engine = std::default_random_engine(std::random_device()());

	std::uniform_real_distribution<f32> Eggshell::rotationDistribution = std::uniform_real_distribution<f32>(0, CNGE::PI * 2);
	std::uniform_real_distribution<f32> Eggshell::speedDistribution = std::uniform_real_distribution<f32>(64.f, 128.f);

	Eggshell::Eggshell(Vector2f position)
		: position(position), velocity(Vector2f(1, 0).rotate(rotationDistribution(engine))* speedDistribution(engine)),
		rotation(rotationDistribution(engine)) {}

	auto Eggshell::update(CNGE::Timing* timing) -> void {
		velocity.translateY(-512 * timing->time);

		position += velocity * (f32)timing->time;
	}

	auto Eggshell::render(CNGE::Camera* camera) -> void {
		Res::eggShader.enable(CNGE::Transform::toModel(position.x() - WIDTH / 2, position.y() - HEIGHT / 2, rotation, WIDTH, HEIGHT), camera->getProjview());
		Res::eggShader.giveColor(1, 1, 1, 1);
		Res::eggshellShape.render();
	}
}
