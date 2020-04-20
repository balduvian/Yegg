
#include "bullet.h"
#include "../res.h"

namespace Game {
	std::vector<Hitbox> Bullet::hitbox = { {Hitbox(Vector2f{0.f, 0.f}, SIZE / 2)} };
	CCDEngine Bullet::ccd = CCDEngine();

	Bullet::Bullet(Vector2f position, f32 direction)
		: position(position), direction(direction) {}

	Bullet::Bullet(const Bullet& other)
		: position(other.position), direction(other.direction) {}

	auto Bullet::update(CNGE::Timing* timing, BlockMap* map) -> bool {
		auto velocity = Vector2f(SPEED, 0).rotate(direction) * (f32)timing->time;

		ccd.start(hitbox, CCDLine(position, velocity + position), map);
		auto collision = ccd.getCollision();
		//collision = nullptr;
		if (collision) {
			return true;

		} else {
			position += velocity;

			return false;
		}
	}

	auto Bullet::renderCircle(CNGE::Camera* camera, Glitch* glitch, CNGE::Color* color) -> void {
		auto offset = glitch->getOffset();

		Res::circleShader.enable(CNGE::Transform::toModel(position.x() - SIZE / 2 + offset.x(), position.y() - SIZE / 2 + offset.y(), 0, SIZE, SIZE), camera->getProjview());
		Res::circleShader.giveColor(*color);
		Res::rect.render();
	}

	auto Bullet::render(CNGE::Camera* camera, Glitch* cyan, Glitch* magenta, Glitch* yellow) -> void {
		Res::circleShader.enable(CNGE::Transform::toModel(position.x() - SIZE / 2, position.y() - SIZE / 2, 0, SIZE, SIZE), camera->getProjview());
		Res::circleShader.giveColor(0, 0, 0, 1);
		Res::rect.render();

		renderCircle(camera, cyan, &BlockMap::cyan);
		renderCircle(camera, magenta, &BlockMap::magenta);
		renderCircle(camera, yellow, &BlockMap::yellow);
	}
}