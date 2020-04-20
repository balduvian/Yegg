
#ifndef CNGE_FIRE
#define CNGE_FIRE

#include "../Types.h"
#include "../engine/loop.h"
#include "../engine/input.h"
#include "../math/Vector2.h"
#include "../engine/camera.h"
#include "../graphics/rect.h"
#include "../graphics/textureShader.h"
#include "../engine/tileGrid.h"

namespace CNGE {
	class Fire {
	public:
		constexpr static f32 HITBOX_RIGHT = 12_f32 / 16_f32;
		constexpr static f32 HITBOX_TOP = 12_f32 / 16_f32;
		constexpr static f32 HITBOX_LEFT = 2_f32 / 16_f32;
		constexpr static f32 HITBOX_BOTTOM = 2_f32 / 16_f32;

	private:
		constexpr static u32 NUM_FRAMES = 16_u32;
		constexpr static f64 ANIMATION_SPEED = 0.04166666666_f32;

		i32 frame;
		f64 timer;

		Vector2f position;

		i32 strength;

	public:
		Fire(f32 x, f32 y, i32 strength);

		//Fire(const Fire& fire);
		//Fire(const Fire&& fire);
		//Fire& operator=(const Fire& other);
		//Fire& operator=(Fire&& other);

		void update(Timing* timing, Input* input);

		void render(Camera* camera, Rect* rect, TextureShader* textureShader, TileGrid* fireTexture);

		f32 getX() const;
		f32 getY() const;

		void takeFrom();
		bool getShouldDie();

	};

	class FireHolder {
	private:
		u32 length;
		u32 firstAvailable;
		u32 count;
		Fire** array;

	public:
		FireHolder(u32 length);

		~FireHolder();

		void add(Fire&& fire);
		void clear();

		void iterateKill(std::function<bool(Fire*)> func);
		void iterate(std::function<void(Fire*)> func);
		bool iterateExit(Fire* &outPtr, std::function<bool(Fire*)> func);

	};
}

#endif
