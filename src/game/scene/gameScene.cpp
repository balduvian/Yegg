
#include "gameScene.h"

#include "math/math.h"
#include "engine/transform.h"
#include "../res.h"

#include <iostream>

namespace Game {

	BlockMap GameScene::maps[NUM_MAPS] = {
		BlockMap("res/level/yegg.png"),
		BlockMap("res/level/level0.png"),
		BlockMap("res/level/level1.png"),
		BlockMap("res/level/level2.png"),
		BlockMap("res/level/level3.png"),
		BlockMap("res/level/level4.png"),
		BlockMap("res/level/level5.png"),
		BlockMap("res/level/end.png")
	};

	const CNGE::Color GameScene::skekColor = CNGE::Color(0x31d433);

	GameScene::GameScene() : Scene(&Res::gameResources),
		aspect(BlockMap::TILE_SIZE * 32, BlockMap::TILE_SIZE * 18, BlockMap::TILE_SIZE * 43, BlockMap::TILE_SIZE * 18),
		camera(),
		currentMap(),
		egg(),
		level(0),
		eggshells(),
		deathTimer(),
		winTimer(),
		bullets()
	{}

	auto GameScene::start() -> void {
		maps[0].hardLoad();
		beginLevel(nullptr, 0);
	}

	auto GameScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameScene::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		nextMap->loadUpdate();

		currentMap->update(timing, bullets);

		if (egg) {
			egg->update(input, timing, currentMap, &aspect, &camera);
			/* center the camera on the egg */
			if (level != NUM_MAPS - 1)
				camera.transform.translation.set(egg->getPosition() - Vector2f(camera.width / 2, camera.height / 2));

			auto bulletIterator = bullets.begin();
			while (bulletIterator != bullets.end()) {
				if (bulletIterator->update(timing, currentMap))
					bulletIterator = bullets.erase(bulletIterator);
				else {
					/* player collision */
					if (egg) {
						for (auto& hitbox : egg->hitboxes) {
							if ((egg->getPosition() + hitbox.getPosition() - bulletIterator->position).getLength() <= hitbox.getRadius() + Bullet::SIZE / 2) {
								die();
								break;
							}
						}
					}

					++bulletIterator;
				}
			}

			if (egg) {
				if (egg->getShouldDie()) {
					die();

				}
				else if (egg->getShouldWin() && !winTimer.timing) {
					startTimer(winTimer, WIN_TIME);
				}
			}
		}

		for (auto& eggshell : eggshells) {
			eggshell.update(timing);
		}

		if (timerUpdate(deathTimer, timing)) {
			beginLevel(nullptr, level);
		}

		if (timerUpdate(winTimer, timing)) {
			beginLevel(currentMap, getNextLevel(level));
		}

		if (timerUpdate(splashTimer, timing)) {
			beginLevel(currentMap, getNextLevel(level));
		}

		camera.update();
	}

	auto GameScene::render() -> void {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		Res::colorShader.enable();
		Res::colorShader.giveColor(1, 1, 1, 1);
		Res::rect.render();

		for (auto& bullet : bullets) {
			bullet.render(&camera, &currentMap->cyanGlitch, &currentMap->magentaGlitch, &currentMap->yellowGlitch);
		}

		currentMap->render(&camera);

		if (egg) {
			egg->render(&camera);
		}

		for (auto& eggshell : eggshells) {
			eggshell.render(&camera);
		}
	}

	auto GameScene::beginLevel(BlockMap* previous, i32 levelIndex) -> void {
		/* we don't need this anymore */
		/* can be nullptr if no previous level */
		if (previous)
			previous->unload();

		level = levelIndex;
		currentMap = maps + level;
		nextMap = maps + getNextLevel(level); 
		/* the next map will be ready by the time this one is over */
		nextMap->load();

		/* init our egg for the level */
		eggshells.clear();
		bullets.clear();

		if (level == 1) {
			Res::songSound.loop(true);
			Res::songSound.play();
		}

		/* don't spawn player in splash screen */
		if (currentMap->dummy) {
			startTimer(splashTimer, SPLASH_TIME);
			camera.transform.translation.setXY(0.f, 0.f);

		} else {
			egg = std::make_unique<Egg>(currentMap->getEggSpawn());
			egg->setAnchor(currentMap->getAnchorSpawn());

			if (level == NUM_MAPS - 1)
				camera.transform.translation.setXY(0.f, 0.f);
			else
				camera.transform.translation.set(egg->getPosition() - Vector2f(camera.width / 2, camera.height / 2));
		}
	}

	auto GameScene::getNextLevel(i32 levelIndex) -> i32 {
		auto ret = levelIndex + 1;

		if (ret == NUM_MAPS) {
			ret = 0;
		}

		return ret;
	}

	auto GameScene::startTimer(Timer& timer, f32 time) -> void {
		timer.timing = true;
		timer.timer = time;
	}

	auto GameScene::stopTimer(Timer& timer) -> void {
		timer.timing = false;
	}

	auto GameScene::die() -> void {
		for (auto i = 0; i < 6; ++i) {
			eggshells.emplace_back(egg->getPosition());
		}

		Res::deathSound.loop(false);
		Res::deathSound.play();

		egg = nullptr;

		startTimer(deathTimer, DEATH_TIME);
		/* you can't win after you die */
		stopTimer(winTimer);
	}

	auto GameScene::timerUpdate(Timer& timer, CNGE::Timing* timing) -> bool {
		if (timer.timing) {
			timer.timer -= timing->time;

			if (timer.timer <= 0) {
				timer.timing = false;
				timer.timer = 0;

				return true;
			}
		}

		return false;
	}

}
