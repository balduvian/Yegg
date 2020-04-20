
#include <iostream>

#include "../load/resource.h"

#include "SceneManager.h"

namespace CNGE {
	SceneManager::SceneManager()
		: isLoading(false), scene(), loadScreen() {}
	
	auto SceneManager::start(Input* input, std::unique_ptr<Scene>&& initialScene, std::unique_ptr<LoadScreen>&& initialLoadScreen) -> void {
		isLoading = true;

		/* set the scene and loadScreen ftom what we passed in */
		scene = std::move(initialScene);
		loadScreen = std::move(initialLoadScreen);

		scene->getBundle()->load();

		/* setup the load screen for the current window */
		loadScreen->resized(input->getWidth(), input->getHeight());
	}

	auto SceneManager::updateLoading(Input* input, Timing* timing) -> bool {
		/* resize the loading screen */
		if (input->getResized())
			loadScreen->resized(input->getWidth(), input->getHeight());

		/* advance loading along */
		auto bundle = scene->getBundle();
		bundle->update();

		/* when the loading screen is done */
		if (bundle->getDone()) {
			/* setup the scene for the current window */
			scene->resized(input->getWidth(), input->getHeight());

			scene->start();
			
			return false;

		} else {
			/* update the loading screen */
			loadScreen->update(input, timing);
			loadScreen->render(bundle->getCurrent(), bundle->getAlong(), bundle->getTotal());
		}

		return true;
	}

	auto SceneManager::updateScene(Input* input, Timing* timing) -> bool {
		/* resize the scene with the window */
		if (input->getResized())
			scene->resized(input->getWidth(), input->getHeight());

		scene->update(input, timing);
		scene->render();

		// check if we need to switch scenes
		if (scene->getSwitching()) {
			auto pair = onSceneSwitch(scene->getSwitchCode());

			start(input, std::move(pair.first), std::move(pair.second));

			return true;
		}

		return false;
	}

	/// manages the updating of both loading screens and scenes
	/// updates and renders
	auto SceneManager::update(Input* input, Timing* timing) -> void {
		/* loading assets and rendering loading screen */
		if (isLoading)
			isLoading = updateLoading(input, timing);
		
		/* updating and rendering normally */
		if (!isLoading)
			isLoading = updateScene(input, timing);

		/* check to load yet again */
		if (isLoading)
			isLoading = updateLoading(input, timing);
	}

}
