
#include "glitch.h"
#include "math/math.h"

namespace Game {
	std::default_random_engine Glitch::engine = std::default_random_engine(std::random_device()());

	std::uniform_real_distribution<f32> Glitch::rotationDistribution = std::uniform_real_distribution<f32>(0, CNGE::PI * 2);
	std::uniform_real_distribution<f32> Glitch::distanceDistribution = std::uniform_real_distribution<f32>(-MAX_RANGE, MAX_RANGE);
	std::uniform_real_distribution<f32>    Glitch::timerDistribution = std::uniform_real_distribution<f32>(0, MAX_TIMER);
	std::uniform_real_distribution<f32>    Glitch::speedDistribution = std::uniform_real_distribution<f32>(-MAX_SPEED, MAX_SPEED);

	std::uniform_int_distribution<i32>      Glitch::boolDistribution =  std::uniform_int_distribution<i32>(0, 1);

	Glitch::Glitch() :
		rotation(0.f),
		distance(0.f),
		timer(0.f),
		speed(0.f)
	{}

	auto Glitch::reset() -> void {
		rotation = rotationDistribution(engine);
		distance = distanceDistribution(engine);
		timer = timerDistribution(engine);
		speed = speedDistribution(engine);
	}

	auto Glitch::update(CNGE::Timing* timing) -> void {
		timer -= timing->time;

		/* time end randomizes again */
		if (timer <= 0) {
			reset();

		/* else use speed to move values continuously */
		} else {
			if (boolDistribution(engine))
				speed += MAX_SPEED / 2 * timing->time;
			else
				speed -= MAX_SPEED / 2 * timing->time;

			if (speed > MAX_SPEED)
				speed = MAX_SPEED;
			if (speed < -MAX_SPEED)
				speed = -MAX_SPEED;

			rotation += speed * timing->time;

			distance += speed * timing->time;

			if (distance > MAX_RANGE)
				distance = MAX_RANGE;
			if (distance < -MAX_RANGE)
				distance = -MAX_RANGE;
		}

	}

	auto Glitch::getOffset() -> Vector2f {
		return Vector2f(distance, 0).rotate(rotation);
	}
}
