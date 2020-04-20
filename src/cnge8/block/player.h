
#pragma once

#ifndef CNGE_PLAYER
#define CNGE_PLAYER

#include "../engine/input.h"
#include "../engine/loop.h"
#include "map.h"
#include "fire.h"

namespace CNGE {
	class Player {
	private:
		constexpr static f32 FIRE_REACH = 1_f32;

		constexpr static f32 ACCELERATION = 16_f32;
		constexpr static f32 MAX_SPEED = 4_f32;

		constexpr static f32 HITBOX_RIGHT = 13_f32 / 16_f32;
		constexpr static f32 HITBOX_TOP = 13_f32 / 16_f32;
		constexpr static f32 HITBOX_LEFT = 3_f32 / 16_f32;
		constexpr static f32 HITBOX_BOTTOM = 0_f32;

		enum class Direction {
			RIGHT, UP, LEFT, DOWN
		};

		bool carrying;
		bool createFire;
		Vector2f firePos;

		f32 angle;

		Vector2f position;
		Vector2f velocity;

		Direction direction;

	public:
		Player(f32 x, f32 y);

		void update(Timing* timing, Input* input, Map* map, FireHolder* fires);

		void render(Camera* camera, Rect* rect, TextureShader* textureShader, TileGrid* playerTexture);

		f32 getX();
		f32 getY();

		bool shouldCreateFire();
		Vector2f getFirePos();

	};

}

#endif
