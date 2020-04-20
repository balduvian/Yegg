
#include "player.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "../math/Vector.h"
#include <math.h>
#include <iostream>
#include <limits>
#include "ccd.h"

namespace CNGE {
	Player::Player(f32 x, f32 y) : position(x, y), velocity(0_f32, 0_f32), direction(Direction::DOWN), carrying(false), angle(-M_PI / 2_f32), createFire(false), firePos(0_f32, 0_f32) {

	}

	void Player::update(Timing* timing, Input* input, Map* map, FireHolder* fires) {
		auto currentHorz = 0_f32;
		auto currentVert = 0_f32;

		// find the diraction our keys are currently moving us in
		if (input->getKeyDown(GLFW_KEY_W))
			++currentVert;

		if (input->getKeyDown(GLFW_KEY_A))
			--currentHorz;

		if (input->getKeyDown(GLFW_KEY_S))
			--currentVert;

		if (input->getKeyDown(GLFW_KEY_D))
			++currentHorz;

		// this is the velocity we are adding this frame
		auto addVector = Vector2f{ currentHorz, currentVert } * ACCELERATION * (f32)timing->time;

		velocity += addVector;

		/// * FRICTION *
		if (velocity.getX() != 0 || velocity.getY() != 0) {
			auto frictStrength = (addVector.getX() != 0 || addVector.getY() != 0)
				? angleBetween<2, f32>(addVector, velocity) / M_PI
				: 1;

			auto friction = (Vector2f{ velocity }.normalize() * ACCELERATION * (f32)frictStrength * (f32)timing->time);
			if (isnan(friction.getX() + friction.getY())) {
				friction = Vector2f{ 0_f32, 0_f32 };
			}

			//make sure we don't go past the 0 point with friction
			if (friction.getLength() > velocity.getLength()) {
				velocity.setXY(0, 0);
			}
			else {
				velocity -= friction;
			}
			/// * FRICTION *

			// make sure we don't go too fast
			if (velocity.getLength() > MAX_SPEED) {
				velocity.setLength(MAX_SPEED);
			}

			// determine direction
			if (addVector.getX() != 0 || addVector.getY() != 0) {
				angle = addVector.getAngle();

				if (angle > 0) {
					if (angle < M_PI / 4_f32) {
						direction = Direction::RIGHT;
					} else if (angle < M_PI * 3_f32 / 4_f32) {
						direction = Direction::UP;
					} else {
						direction = Direction::LEFT;
					}
				} else {
					if (angle > -M_PI / 4_f32) {
						direction = Direction::RIGHT;
					} else if (angle > -M_PI * 3_f32 / 4_f32) {
						direction = Direction::DOWN;
					} else {
						direction = Direction::LEFT;
					}
				}
			}

			// NOW TIME FOR CCD GET HYPE
			struct CheckPass {
				CheckPass(Vector2f startP, Vector2f endP, f32 offX, f32 offY)
					: offsetVector(offX, offY), start(startP + offsetVector), end(endP + offsetVector), boundingBox(start, end) {}

				Vector2f offsetVector, start, end;
				BoundingBox boundingBox;
			};

			CollStruct horzStruct;
			CollStruct vertStruct;

			auto checkRight = [map, &horzStruct, fires](CheckPass& pass) {
				for (auto i = pass.boundingBox.minX; i <= pass.boundingBox.maxX; ++i) {
					for (auto j = pass.boundingBox.minY; j <= pass.boundingBox.maxY; ++j) {
						
						if (map->getRightLine(i, j))
							horizontalColl(horzStruct, pass.start, pass.end, HITBOX_RIGHT + CCD_SKEK, i + 1, j, j + 1);
					}
				}

				fires->iterate([&horzStruct, &pass](Fire* fire) {
					horizontalColl(horzStruct, pass.start, pass.end, HITBOX_RIGHT + CCD_SKEK, fire->getX() + Fire::HITBOX_LEFT, fire->getY() + Fire::HITBOX_BOTTOM, fire->getY() + Fire::HITBOX_TOP);
				});
			};

			auto checkLeft = [map, &horzStruct, &fires](CheckPass& pass) {
				for (auto i = pass.boundingBox.minX; i <= pass.boundingBox.maxX; ++i) {
					for (auto j = pass.boundingBox.minY; j <= pass.boundingBox.maxY; ++j) {

						if (map->getLeftLine(i, j))
							horizontalColl(horzStruct, pass.start, pass.end, HITBOX_LEFT - CCD_SKEK, i, j, j + 1);
					}
				}

				fires->iterate([&horzStruct, &pass](Fire* fire) {
					horizontalColl(horzStruct, pass.start, pass.end, HITBOX_LEFT - CCD_SKEK, fire->getX() + Fire::HITBOX_RIGHT, fire->getY() + Fire::HITBOX_BOTTOM, fire->getY() + Fire::HITBOX_TOP);
				});
			};

			auto checkUp = [map, &vertStruct, &fires](CheckPass& pass) {
				for (auto i = pass.boundingBox.minX; i <= pass.boundingBox.maxX; ++i) {
					for (auto j = pass.boundingBox.minY; j <= pass.boundingBox.maxY; ++j) {

						if (map->getTopLine(i, j))
							verticalColl(vertStruct, pass.start, pass.end, HITBOX_TOP + CCD_SKEK, j + 1, i, i + 1);
					}
				}

				fires->iterate([&vertStruct, &pass](Fire* fire) {
					verticalColl(vertStruct, pass.start, pass.end, HITBOX_TOP + CCD_SKEK, fire->getY() + Fire::HITBOX_BOTTOM, fire->getX() + Fire::HITBOX_LEFT, fire->getX() + Fire::HITBOX_RIGHT);
				});
			};

			auto checkDown = [map, &vertStruct, &fires](CheckPass& pass) {
				for (auto i = pass.boundingBox.minX; i <= pass.boundingBox.maxX; ++i) {
					for (auto j = pass.boundingBox.minY; j <= pass.boundingBox.maxY; ++j) {

						if (map->getBottomLine(i, j))
							verticalColl(vertStruct, pass.start, pass.end, HITBOX_BOTTOM - CCD_SKEK, j, i, i + 1);
					}
				}

				fires->iterate([&vertStruct, &pass](Fire* fire) {
					verticalColl(vertStruct, pass.start, pass.end, HITBOX_BOTTOM - CCD_SKEK, fire->getY() + Fire::HITBOX_TOP, fire->getX() + Fire::HITBOX_LEFT, fire->getX() + Fire::HITBOX_RIGHT);
				});
			};

			// without any other modifications
			// this is the start and end of your movement
			auto startP = position;
			auto endP = position + (velocity * (f32)timing->time);

			// which direction we are moving determines what we can collide with
			auto movingRight = endP.getX() > startP.getX();
			auto movingUp = endP.getY() > startP.getY();
			auto movingLeft = endP.getX() < startP.getX();
			auto movingDown = endP.getY() < startP.getY();

			auto topRight = CheckPass(startP, endP, HITBOX_RIGHT, HITBOX_TOP);
			auto bottomRight = CheckPass(startP, endP, HITBOX_RIGHT, HITBOX_BOTTOM);
			auto topLeft = CheckPass(startP, endP, HITBOX_LEFT, HITBOX_TOP);
			auto bottomLeft = CheckPass(startP, endP, HITBOX_LEFT, HITBOX_BOTTOM);

			if (movingRight) {
				checkRight(topRight);
				checkRight(bottomRight);

			} else if (movingLeft) {
				checkLeft(topLeft);
				checkLeft(bottomLeft);

			}

			if (movingUp) {
				checkUp(topRight);
				checkUp(topLeft);

			} else if (movingDown) {
				checkDown(bottomRight);
				checkDown(bottomLeft);

			}

			if (horzStruct.coll) {
				velocity.setX(0);
				position.setX(horzStruct.collPos);
			} else
				position.setX(endP.getX());

			if (vertStruct.coll) {
				velocity.setY(0);
				position.setY(vertStruct.collPos);
			} else
				position.setY(endP.getY());

			/// NOW FIRE INTERACTION

			auto endP = position + (velocity * (f32)timing->time);
			position.setX(endP.getX());
			position.setY(endP.getY());
		}

		if (input->getKeyPressed(GLFW_KEY_SPACE) == Input::BUTTON_PRESSED) {
			auto fireReach = Vector2f(1_f32, 0_f32).rotate(angle) * FIRE_REACH;

			// start from the center of our player
				// and go out from there
			auto start = position + Vector2f(0.5_f32, 0.5_f32);
			auto end = start + fireReach;

			// do ccd to see if we can place the fire
			auto boundingBox = BoundingBox(start, end);

			auto movingRight = end.getX() > start.getX();
			auto movingUp = end.getY() > start.getY();
			auto movingLeft = end.getX() < start.getX();
			auto movingDown = end.getY() < start.getY();

			if (carrying) {
				auto checkRight = [map, start, end, boundingBox]() {
					for (auto i = boundingBox.minX; i <= boundingBox.maxX; ++i) {
						for (auto j = boundingBox.minY; j <= boundingBox.maxY; ++j) {
							if (map->getRightLine(i, j) && horizontalTest(start, end, i + 1, j, j + 1))
								return true;
						}
					}

					return false;
				};

				auto checkLeft = [map, start, end, boundingBox]() {
					for (auto i = boundingBox.minX; i <= boundingBox.maxX; ++i) {
						for (auto j = boundingBox.minY; j <= boundingBox.maxY; ++j) {
							if (map->getLeftLine(i, j) && horizontalTest(start, end, i, j, j + 1))
								return true;
						}
					}

					return false;
				};

				auto checkUp = [map, start, end, boundingBox]() {
					for (auto i = boundingBox.minX; i <= boundingBox.maxX; ++i) {
						for (auto j = boundingBox.minY; j <= boundingBox.maxY; ++j) {
							if (map->getTopLine(i, j) && verticalTest(start, end, j + 1, i, i + 1))
								return true;
						}
					}

					return false;
				};

				auto checkDown = [map, start, end, boundingBox]() {
					for (auto i = boundingBox.minX; i <= boundingBox.maxX; ++i) {
						for (auto j = boundingBox.minY; j <= boundingBox.maxY; ++j) {
							if (map->getBottomLine(i, j) && verticalTest(start, end, j, i, i + 1))
								return true;
						}
					}

					return false;
				};

				// this is if our fire reach hits a wall
				if (
					!((movingRight && checkRight())
					|| (movingLeft && checkLeft())
					|| (movingUp && checkUp())
					|| (movingDown && checkDown()))
				) {
					carrying = false;
					createFire = true;

					// place the fire at the end of our reach
					firePos = position + fireReach;
				}
			}
			else {
				Fire* particularFire;

				auto checkRight = [map, start, end, boundingBox, &particularFire, &fires]() {
					return fires->iterateExit(particularFire, [start, end](Fire* fire) {
						return horizontalTest(start, end, fire->getX() + Fire::HITBOX_LEFT, fire->getY() + Fire::HITBOX_BOTTOM, fire->getY() + Fire::HITBOX_TOP);
					});
				};

				auto checkLeft = [map, start, end, boundingBox, &particularFire, &fires]() {
					return fires->iterateExit(particularFire, [start, end](Fire* fire) {
						return horizontalTest(start, end, fire->getX() + Fire::HITBOX_RIGHT, fire->getY() + Fire::HITBOX_BOTTOM, fire->getY() + Fire::HITBOX_TOP);
					});
				};

				auto checkUp = [map, start, end, boundingBox, &particularFire, &fires]() {
					return fires->iterateExit(particularFire, [start, end](Fire* fire) {
						return verticalTest(start, end, fire->getY() + Fire::HITBOX_BOTTOM, fire->getX() + Fire::HITBOX_LEFT, fire->getX() + Fire::HITBOX_RIGHT);
					});
				};

				auto checkDown = [map, start, end, boundingBox, &particularFire, &fires]() {
					return fires->iterateExit(particularFire, [start, end](Fire* fire) {
						return verticalTest(start, end, fire->getY() + Fire::HITBOX_TOP, fire->getX() + Fire::HITBOX_LEFT, fire->getX() + Fire::HITBOX_RIGHT);
					});
				};

				// this is if our reach hits a fire
				if (
					((movingRight && checkRight())
					|| (movingLeft && checkLeft())
					|| (movingUp && checkUp())
					|| (movingDown && checkDown()))
				) {
					carrying = true;

					// take from the fire we found
					particularFire->takeFrom();
				}
			}
		}
	}

	bool Player::shouldCreateFire() {
		if (createFire) {
			createFire = false;
			return true;
		}

		return false;
	}

	Vector2f Player::getFirePos() {
		return firePos;
	}

	void Player::render(Camera* camera, Rect* rect, TextureShader* textureShader, TileGrid* playerTexture) {
		playerTexture->bind();
		textureShader->enable(Transform::toModel(position.getX(), position.getY(), 0, 1, 1), camera->getProjview());

		auto sheetX = 0;
		auto sheetY = carrying ? 1 : 0;

		switch (direction) {
		case Direction::RIGHT:
			sheetX = 0;
			break;
		case Direction::UP:
			sheetX = 1;
			break;
		case Direction::LEFT:
			sheetX = 2;
			break;
		case Direction::DOWN:
			sheetX = 3;
			break;
		}

		textureShader->giveParams(1, 1, 1, 1, playerTexture->getSheet(sheetX, sheetY));
		rect->render();
	}

	f32 Player::getX() {
		return position.getX();
	}

	f32 Player::getY() {
		return position.getY();
	}

}
