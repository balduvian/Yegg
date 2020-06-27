
#include "gameLoadScreen.h"
#include "gameResources.h"

#include "../cnge8/engine/transform.h"

namespace Game {
	const CNGE::Color GameLoadScreen::backgroundColor = CNGE::Color(234, 56, 100);
	const CNGE::Color GameLoadScreen::mainColor = backgroundColor.invert();

	GameLoadScreen::GameLoadScreen() 
		: aspect(16_f32, 8_f32, 21.5_f32, 9_f32), camera() {}

	auto GameLoadScreen::resized(i32 width, i32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameLoadScreen::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		camera.update();
	}

	auto GameLoadScreen::render(CNGE::Resource* current, i32 along, i32 total) -> void {
		GameResources::colorShader.enable();
		GameResources::colorShader.giveColor(backgroundColor);
		GameResources::rect.render();

		GameResources::colorShader.enable(CNGE::Transform::toModel(2, 2, 0, 6, 5), camera.getProjview());
		GameResources::colorShader.giveColor(mainColor);
		GameResources::rect.render();
	}
}
