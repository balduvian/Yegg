
#ifndef GAME_BLOCK_MAP
#define GAME_BLOCK_MAP

class Bullet;
class Launcher;

#include <random>
#include <vector>

#include "glitch.h"
#include "engine/camera.h"
#include "block.h"
#include "../load/gameResource.h"
#include "image/image.h"
#include "util/color.h"
#include "engine/loop.h"
#include "rotation.h"
#include "../egg/launcher.h"

namespace Game {
	class BlockMap : public GameResource {
	private:
		/* loading variables */
		const char* filePath;

		/* map variables */
		i32 width;
		i32 height;

		Block*** blocks;
		bool** circles;

		/* spawn variables */
		Vector2f eggSpawn;
		Vector2f anchorSpawn;

		std::vector<Launcher> launchers;

		template <typename T>
		auto tripleAlloc(i32 height, i32 width) -> T*** {
			auto ret = new T**[height];

			for (auto j = 0; j < height; ++j)
				ret[j] = new T * [width];

			return ret;
		}

		template <typename T>
		auto doubleAlloc(i32 height, i32 width) -> T** {
			auto ret = new T*[height];

			for (auto j = 0; j < height; ++j)
				ret[j] = new T[width]();

			return ret;
		}

		template <typename T>
		auto killArray(T*** array) {
			for (auto j = 0; j < height; ++j)
				delete[] array[j];

			delete[] array;
		}

		template <typename T>
		auto killArray(T** array) {
			for (auto j = 0; j < height; ++j)
				delete[] array[j];

			delete[] array;
		}
	
		struct mapPos { i32 x; i32 y; };

		auto outOfMap(i32, i32) -> bool;

		auto putCircle(i32, i32) -> void;

		auto circleRoutine(Rotation) -> void;
		auto shouldPlaceCircle(Rotation, Rotation, Rotation, Rotation, Block*, Block*, Block*, Block*) -> bool;

	public:
		constexpr static f32 TILE_SIZE = 32.f;

		bool dummy;

		static CNGE::Color cyan;
		static CNGE::Color magenta;
		static CNGE::Color yellow;
		static CNGE::Color black;

		BlockMap(const char*);

		auto update(CNGE::Timing*, std::vector<Bullet>&) -> void;

		auto render(CNGE::Camera*) -> void;
		auto renderTiles(CNGE::Camera*, i32, i32, i32, i32, f32, f32, CNGE::Color&, bool) -> void;

		virtual auto customLoad() -> void override;
		virtual auto customUnload() -> void override;
		
		auto getWorldBlock(f32, f32) -> Block*;

		auto worldToMap(f32, f32)->mapPos;
		auto getBlock(i32, i32) -> Block*;
		auto getCircle(i32, i32) -> bool;
		
		auto getEggSpawn() -> Vector2f&;
		auto getAnchorSpawn() -> Vector2f&;

		Glitch cyanGlitch;
		Glitch magentaGlitch;
		Glitch yellowGlitch;

		~BlockMap();
	};
}

#endif
