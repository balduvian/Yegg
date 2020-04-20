
#ifndef CNGE_SCENE_MANAGER
#define CNGE_SCENE_MANAGER

#include <memory>

#include "../Types.h"
#include "Scene.h"
#include "../load/loadScreen.h"
#include "../engine/window.h"

namespace CNGE {
	class SceneManager {
	private:
        bool isLoading;

		std::unique_ptr<Scene> scene;
		std::unique_ptr<LoadScreen> loadScreen;

	protected:
		using SceneSwitchReturn = std::pair<std::unique_ptr<Scene>&&, std::unique_ptr<LoadScreen>&&>;

	public:
		SceneManager();

		auto start(Input*, std::unique_ptr<Scene>&&, std::unique_ptr<LoadScreen>&&) -> void;

		/* sub update funcitons */
		auto updateLoading(Input*, Timing*) -> bool;
		auto updateScene(Input*, Timing*) -> bool;

		auto update(Input*, Timing*) -> void;

        virtual auto onSceneSwitch(i32) -> SceneSwitchReturn = 0;
	};

}

#endif
