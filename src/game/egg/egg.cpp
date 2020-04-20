
#include "egg.h"

#include "../res.h"
#include "math/math.h"
#include "../map/blocks.h"

namespace Game {
	Egg::Egg(Vector2f position)
		: position(position), velocity(0.f, 0.f), rotation(0),
		hitboxes({ {{0.f, -4.5f}, 10}, {{0.f, 2.5f}, 10}, {{0.f, 8.5f}, 6} }),
		anchorHitbox({ {{0.f, 0.f}, 1.f} }),
		anchorPosition(0.f, 0.f), anchored(false), shooting(false), anchorLength(), ccd(),
		shouldDie(false), shouldWin(false) {}

	auto Egg::update(CNGE::Input* input, CNGE::Timing* timing, BlockMap* map, CNGE::Aspect* aspect, CNGE::Camera* camera) -> void {
		velocity.y() -= GRAVITY * timing->time;

		if (input->getMousePressed(GLFW_MOUSE_BUTTON_1) == CNGE::Input::BUTTON_PRESSED) {
			if (anchored) {
				anchored = false;

			/* can't shoot when you're already shooting */
			} else if (!shooting) {
				auto shootX = aspect->screenToGameX(input->getCursorX()) + camera->transform.translation.x();
				auto shootY = aspect->screenToGameY(input->getCursorY()) + camera->transform.translation.y();

				anchorPosition = position;
				shootingAngle = (Vector2f(shootX, shootY) - position).normalize();

				shooting = true;
			}
		}

		if (shooting) {
			auto anchorVelocity = shootingAngle * SHOOTING_SPEED;

			ccd.start(anchorHitbox, CCDLine(anchorPosition, anchorPosition + anchorVelocity), map);
			auto collision = ccd.getCollision();

			if (collision) {
				setAnchor(collision->position);

			} else {
				anchorPosition += anchorVelocity;
			}
		}

		if (input->getKeyDown(GLFW_KEY_D)) {
			velocity.translateX(SPEED * timing->time);
			rotation += 0.5f * timing->time;
		}

		if (input->getKeyDown(GLFW_KEY_A)) {
			velocity.translateX(-SPEED * timing->time);
			rotation -= 0.5f * timing->time;
		}

		if (anchored) {
			auto toEggVec = (position - anchorPosition);

			if (toEggVec.getLength() > anchorLength) {
				auto edgePosition = toEggVec.setLength(anchorLength) + anchorPosition;

				auto awayFromEdge = position - edgePosition;
				
				velocity = project(velocity, rotate(toEggVec, CNGE::PI / 2)) - awayFromEdge;
			}
		}

		ccd.start(hitboxes, CCDLine(position, position + velocity), map);

		auto collision = ccd.getCollision();
		if (collision) {
			
			position.setXY(collision->position);

			velocity = project(velocity, collision->wallVector).scale(1 - 0.9f * timing->time);

			/* kill the player for touching a wall */
			/* win for touching the exit */
			if (collision->block == &Blocks::goodBlock) {
				shouldWin = true;
			} else {
				shouldDie = true;
			}
		}
		else {
			
			position.translate(velocity);
		}

		
	}

	CNGE::Color Egg::color0 = CNGE::Color(255, 0, 50);
	CNGE::Color Egg::color1 = CNGE::Color(50, 255, 0);
	CNGE::Color Egg::color2 = CNGE::Color(0, 50, 255);

	auto Egg::renderHitBox(CNGE::Camera* camera, CNGE::Color& color, Hitbox& hitbox) -> void {
		auto radius = hitbox.getRadius();
		auto boxPos = position + hitbox.getPosition() - Vector2f(radius, radius);

		Res::circleShader.enable(CNGE::Transform::toModel(boxPos.x(), boxPos.y(), 0, radius * 2, radius * 2), camera->getProjview());
		Res::circleShader.giveColor(color);
		Res::rect.render();
	}

	auto Egg::render(CNGE::Camera* camera) -> void {
		/* render hitbox */
		//renderHitBox(camera, color0, hitboxes[0]);
		//renderHitBox(camera, color1, hitboxes[1]);
		//renderHitBox(camera, color2, hitboxes[2]);

		/* render rope */
		if (anchored || shooting) {
			auto ropeVec = (anchorPosition - position);
			auto ropeWidth = 8;

			auto ropeAngle = ropeVec.getAngle();

			auto boxPos = position + Vector2f(ropeWidth / 2, 0).rotate(ropeAngle - CNGE::PI / 2);
			
			Res::ropeTexture.bind();
			Res::textureShader.enable(CNGE::Transform::toModel(boxPos.x(), boxPos.y(), ropeAngle, ropeVec.getLength(), ropeWidth), camera->getProjview());
			Res::textureShader.giveParams();
			Res::rect.render();
		}

		/* render egg */
		//auto boxPos = position - Vector2f(BOX_WIDTH / 2, BOX_HEIGHT / 2);

		Res::eggShader.enable(CNGE::Transform::toModel(position.x(), position.y(), 0, 1, 1), camera->getProjview());
		Res::eggShader.giveColor(1, 1, 1, 1);
		Res::eggShape.render();

		//Res::eggTexture.bind();
		//Res::textureShader.enable(CNGE::Transform::toModel(boxPos.x(), boxPos.y(), 0, BOX_WIDTH, BOX_HEIGHT), camera->getProjview());
		//Res::textureShader.giveParams();
		//Res::rect.render();
	}

	auto Egg::setAnchor(Vector2f setPosition) -> void {
		anchorPosition = setPosition;
		shooting = false;
		anchored = true;

		anchorLength = (anchorPosition - position).getLength();
	}

	auto Egg::getPosition() -> Vector2f {
		return position;
	}

	auto Egg::getShouldDie() -> bool {
		return shouldDie;
	}

	auto Egg::getShouldWin() -> bool {
		return shouldWin;
	}
}
