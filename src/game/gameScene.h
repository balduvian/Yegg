
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "engine/camera.h"
#include "util/aspect.h"
#include "../cnge8/scene/scene.h"
#include "../cnge8/util/color.h"
#include "player.h"

#include "map/lineLoop.h"

namespace Game {
	class GameScene : public CNGE::Scene {
	private:
		const static CNGE::Color skekColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;

		LineLoop loop;
		Player player;

		f32 angle;
		
	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;
	};
}

#endif
