
#include "gameScene.h"

#include "math/math.h"
#include "engine/transform.h"
#include "gameResources.h"

#include <iostream>

namespace Game {
	const CNGE::Color GameScene::skekColor = CNGE::Color(0x31d433);

	GameScene::GameScene() : Scene(&GameResources::gameResources),
		loop({ {2, 4}, {6, 4}, {8, 5}, {10, 5}, {12, 3}, {14, 3}, {14, 1}, {2, 1} }),
		//loop({ {11, 3}, {10, 7} }, false),
		aspect(16, 9, 21, 9),
		camera(),
		angle(0),
		player(5, 6)
	{
		loop.get(0)->name = "left floor";
		loop.get(1)->name = "left ramp";
		loop.get(2)->name = "mountain top";
		loop.get(3)->name = "right ramp";
		loop.get(4)->name = "right floor";
		loop.get(5)->name = "right wall";
		loop.get(6)->name = "bottom";
		loop.get(7)->name = "left wall";
	}

	auto GameScene::start() -> void {
		GameResources::testSound.loop(true);
		GameResources::testSound.play();
	}

	auto GameScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameScene::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		camera.update();

		player.update(input, timing, &loop);

		if (input->getKeyDown(GLFW_KEY_W)) {
			angle += timing->time * (CNGE::PI / 2.f);
		}

		if (input->getKeyDown(GLFW_KEY_S)) {
			angle -= timing->time * (CNGE::PI / 2.f);
		}

		angle = CNGE::mod<f32>(angle, CNGE::PI * 2.f);
	}

	auto GameScene::render() -> void {
		GameResources::colorShader.enable();
		GameResources::colorShader.giveColor(skekColor);
		GameResources::rect.render();

		auto invColor = skekColor.invert();
		auto startColor = CNGE::Color(255, 23, 12);
		auto endColor = CNGE::Color(10, 200, 75);

		auto halfWidth = 0.1f;

		GameResources::colorShader.enable(CNGE::Transform::toModel(0, 0, angle, 1, 1), camera.getProjview());
		GameResources::colorShader.giveColor(invColor);
		GameResources::rect.render();

		for (auto &i : loop) {
			auto lineStart = i.start;
			auto lineEnd = i.end;
			auto lineVec = (lineEnd - lineStart);

			auto length = lineVec.getLength();
			auto perpVec = Vector2f(halfWidth, 0).rotate(lineVec.getAngle() - (CNGE::PI / 2)) + lineStart;

			GameResources::colorShader.enable(CNGE::Transform::toModel(lineStart.x() - 0.25f, lineStart.y() - 0.25f, 0, 0.5f, 0.5f), camera.getProjview());
			GameResources::colorShader.giveColor(startColor);
			GameResources::rect.render();

			GameResources::colorShader.enable(CNGE::Transform::toModel(perpVec.x(), perpVec.y(), lineVec.getAngle(), length, halfWidth * 2), camera.getProjview());
			GameResources::colorShader.giveColor(invColor);
			GameResources::rect.render();

			//std::cout << "{" << lineStart << ", " << lineEnd << "}" << std::endl;
		}

		player.render(&camera);

		//auto i = 0;
	}
}
