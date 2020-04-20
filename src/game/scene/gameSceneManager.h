
#ifndef GAME_GAME_SCENE_MANAGER
#define GAME_GAME_SCENE_MANAGER

#include "../cnge8/scene/sceneManager.h"

namespace Game {
	class GameSceneManager : public CNGE::SceneManager {
	public:
		GameSceneManager();

		auto onSceneSwitch(i32) -> SceneSwitchReturn override;

	private:



	};

}

#endif
