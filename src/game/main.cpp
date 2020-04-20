
#include <iostream>
#include <functional>

#include "../cnge8/engine/window.h"
#include "../cnge8/engine/loop.h"
#include "audio/audioDevice.h"

#include "res.h"
#include "scene/gameSceneManager.h"
#include "scene/gameScene.h"
#include "gameLoadScreen.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char* argv) {
	/* init glfw and opengl */
	CNGE::Window::init();
	auto window = CNGE::Window(3, 3, true, true, "Yegg", CNGE::Window::getPrimaryMonitor(), false, true);
	CNGE::Window::wrangle();

	/* init openal */
	CNGE::AudioDevices::init();
	CNGE::AudioDevices::getDefaultOutputDevice()->setActive();

	/* open enables */
	//glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// get load screen ready
	Game::Res::loadScreenResources.load(true).quickUpdate();

	auto sceneManager = Game::GameSceneManager();

	sceneManager.start(window.getInput(), std::make_unique<Game::GameScene>(), std::make_unique<Game::GameLoadScreen>());

	// start the gameloop
	auto loop = CNGE::Loop(std::bind(&CNGE::Window::getShouldClose, window), [&](CNGE::Timing* timing) {
		window.poll();

		sceneManager.update(window.getInput(), timing);

		window.swap();

	}, window.getRefreshRate());

	return 0;
}
