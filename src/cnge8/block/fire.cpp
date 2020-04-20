
#include "fire.h"
#include <iostream>

namespace CNGE {
	FireHolder::FireHolder(u32 length)
		: length(length), array(new Fire* [length] {}), firstAvailable(0), count(0) {}

	FireHolder::~FireHolder() {
		clear();

		delete[] array;
	}

	void FireHolder::add(Fire&& fire) {
		std::cout << "FA ADD: " << firstAvailable << std::endl;
		array[firstAvailable] = new Fire(fire);
		++count;

		// find next avaliable
		if (count < length) {
			do {
				++firstAvailable;
				firstAvailable %= length;
			} while (array[firstAvailable]);
		}
	}

	void FireHolder::clear() {
		for (auto ptr = array, end = ptr + length; ptr < end; ++ptr) {
			if (*ptr) {
				delete *ptr;
			}
		}

		firstAvailable = 0;
		count = 0;
	}

	void FireHolder::iterateKill(std::function<bool(Fire*)> func) {
		for (auto ptr = array, end = ptr + length; ptr < end; ++ptr) {
			if (*ptr && func(*ptr)) {
				--count;
				delete *ptr;
				*ptr = nullptr;
				firstAvailable = ptr - array;
				std::cout << "FA KILL: " << firstAvailable << std::endl;
			}
		}
	}

	void FireHolder::iterate(std::function<void(Fire*)> func) {
		for (auto ptr = array, end = ptr + length; ptr < end; ++ptr) {
			if (*ptr)
				func(*ptr);
		}
	}

	bool FireHolder::iterateExit(Fire* &outPtr, std::function<bool(Fire*)> func) {
		for (auto ptr = array, end = ptr + length; ptr < end; ++ptr) {
			if (*ptr && func(*ptr)) {
				outPtr = *ptr;
				return true;
			}
		}

		return false;
	}

	Fire::Fire(f32 x, f32 y, i32 strength) : frame(0), position(x, y), strength(strength), timer(0) {

	}
	/*
	Fire::Fire(const Fire& fire) : frame(fire.frame), position(fire.position), strength(fire.strength) {
		std::cout << "COPY" << std::endl;
	}

	Fire::Fire(const Fire&& fire) : frame(fire.frame), position(fire.position), strength(fire.strength) {
		std::cout << "MOVE" << std::endl;
	}

	Fire& Fire::operator=(const Fire& other) {
		if (&other == this)
			return *this;
		
		frame = other.frame;
		position = other.position;
		strength = other.strength;
		timer = other.timer;

		return *this;
	}

	Fire& Fire::operator=(Fire&& other) {
		return *this;
	}
	*/

	void Fire::update(Timing* timing, Input* input) {
		// animate
		timer += timing->time;
		if (timer >= ANIMATION_SPEED) {
			timer -= ANIMATION_SPEED;

			++frame;
			if (frame == NUM_FRAMES)
				frame = 0;
		}

	}

	void Fire::render(Camera* camera, Rect* rect, TextureShader* textureShader, TileGrid* fireTexture) {
		fireTexture->bind();
		textureShader->enable(Transform::toModel(position.getX(), position.getY(), 0, 1, 1), camera->getProjview());

		auto sheetX = frame % 4;
		auto sheetY = frame / 4;

		textureShader->giveParams(1, 1, 1, 1, fireTexture->getSheet(sheetX, sheetY));
		rect->render();
	}

	f32 Fire::getX() const {
		return position.getX();
	}

	f32 Fire::getY() const {
		return position.getY();
	}

	void Fire::takeFrom() {
		--strength;
		std::cout << strength << std::endl;
	}

	bool Fire::getShouldDie() {
		return strength == 0;
	}

}
