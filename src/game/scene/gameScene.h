
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "engine/camera.h"
#include "util/aspect.h"
#include "../cnge8/scene/scene.h"
#include "../cnge8/util/color.h"
#include "../egg/egg.h"
#include "../map/blockMap.h"
#include "../egg/eggShell.h"

namespace Game {
	struct Timer {
		bool timing = false;
		f32 timer = 0;
	};

	class GameScene : public CNGE::Scene {
	private:
		constexpr static f32 DEATH_TIME = 2.f;
		constexpr static f32 WIN_TIME = 2.f;
		constexpr static f32 SPLASH_TIME = 4.f;

		const static CNGE::Color skekColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;

		constexpr static i32 NUM_MAPS = 8;
		static BlockMap maps[NUM_MAPS];
		i32 level;
		BlockMap* currentMap;
		BlockMap* nextMap;

		std::unique_ptr<Egg> egg;
		std::vector<Eggshell> eggshells;
		std::vector<Bullet> bullets;

		Timer deathTimer;
		Timer winTimer;
		Timer splashTimer;

	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;

		auto beginLevel(BlockMap*, i32) -> void;
		auto getNextLevel(i32) -> i32;

		auto die() -> void;

		auto startTimer(Timer&, f32) -> void;
		auto stopTimer(Timer&) -> void;
		auto timerUpdate(Timer&, CNGE::Timing*) -> bool;
	};
}

#endif
