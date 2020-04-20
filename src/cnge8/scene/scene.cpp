
#include "Scene.h"

namespace CNGE {
	Scene::Scene(ResourceBundle* bundle)
		: bundle(bundle), switching(false), switchCode(0) {}

	void Scene::switchScene(i32 code) {
		switching = true;
		switchCode = code;
	}

	bool Scene::getSwitching() {
		return switching;
	}

	int Scene::getSwitchCode() {
		return switchCode;
	}

	auto Scene::getBundle() -> ResourceBundle* {
		return bundle;
	}
}
