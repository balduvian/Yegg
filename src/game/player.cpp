
#include "player.h"

#include <iostream>

#include "gameResources.h"
#include "engine/transform.h"

namespace Game {
	Player::Player(f32 x, f32 y) :
		position(x, y),
		ccd(),
		velocity(0.f, 0.f),
		//offsets({ {0.5f, -0.5f} })
		offsets({ {-0.5f, -0.5f}, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, -0.5f} })
	{}

	auto Player::update(CNGE::Input* input, CNGE::Timing* timing, LineLoop* loop) -> void {
		ccd.setLoop(loop);

		bool leftPress = false;
		bool rightPress = false;

		if (input->getKeyDown(GLFW_KEY_A)) {
			leftPress = true;
		}

		if (input->getKeyDown(GLFW_KEY_D)) {
			rightPress = true;
		} 

		if (input->getKeyDown(GLFW_KEY_W) && onGround) {
			velocity.setY(20 * timing->time);
		}

		if (leftPress && !rightPress) {
			velocity.translateX((f32)(-timing->time * SPEED));
		}

		if (rightPress && !leftPress) {
			velocity.translateX((f32)(timing->time * SPEED));
		}

		if (!leftPress) {
			if (velocity.x() < 0) {
				velocity.translateX(FRICTION * timing->time);

				if (velocity.x() > 0)
					velocity.setX(0);
			}
		}

		if (!rightPress) {
			if (velocity.x() > 0) {
				velocity.translateX(-FRICTION * timing->time);

				if (velocity.x() < 0)
					velocity.setX(0);
			}
		}

		if (velocity.x() > MAX_SPEED * timing->time)
			velocity.setX(MAX_SPEED * timing->time);

		if (velocity.x() < -MAX_SPEED * timing->time)
			velocity.setX(-MAX_SPEED * timing->time);

		velocity.translateY(-GRAVITY * timing->time);

		if (velocity.getY() < -MAX_FALL) {
			velocity.setY(-MAX_FALL);
		}

		/* no collision when we aren't moving */
		if (velocity.getLength() > 0)
			ccd.start(offsets, CCDLine(position, position + velocity));

		auto collision = ccd.getCollision();

		if (collision) {
			position.setXY(collision->position);
			velocity.setY(0);
			onGround = true;
		} else {
			position.translateXY(velocity);
			onGround = false;
		}
	}

	CNGE::Color Player::COLOR = CNGE::Color(34, 104, 145);

	auto Player::render(CNGE::Camera* camera) -> void {
		GameResources::colorShader.enable(CNGE::Transform::toModel(position.x() - 0.5, position.y() - 0.5, 0, 1, 1), camera->getProjview());
		GameResources::colorShader.giveColor(COLOR);
		GameResources::rect.render();
	}
}
