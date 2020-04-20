
#ifndef GAME_GAME_LOAD_SCREEN
#define GAME_GAME_LOAD_SCREEN

#include "../cnge8/load/loadScreen.h"
#include "../cnge8/util/aspect.h"
#include "../cnge8/util/color.h"
#include "../cnge8/engine/camera.h"

namespace Game {
	class GameLoadScreen : public CNGE::LoadScreen {
	public:
		GameLoadScreen();

		auto resized(i32, i32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render(CNGE::Resource*, i32, i32) -> void override;

	private:
		const static CNGE::Color backgroundColor;
		const static CNGE::Color mainColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;
	};
}

#endif
