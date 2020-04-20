
#include "gameResource.h"

namespace Game {
	std::mutex GameResource::mutex = std::mutex();

	GameResource::GameResource() : thread(), loadStatus(LOAD_UNLOADED) {}

	auto GameResource::hardLoad() -> void {
		if (loadStatus == LOAD_UNLOADED) {
			customLoad();

			loadStatus = LOAD_DONE;
		}
	}

	auto GameResource::load() -> void {
		if (loadStatus == LOAD_UNLOADED) {
			thread = std::thread([this]() -> void {
				mutex.lock();

				customLoad();

				mutex.unlock();
				loadStatus = LOAD_DONE;
			});
		}
	}

	auto GameResource::unload() -> void {
		if (loadStatus != LOAD_UNLOADED) {
			customUnload();

			loadStatus = LOAD_UNLOADED;
		}
	}

	auto GameResource::loadUpdate() -> void {
		if (loadStatus == LOAD_DONE) {
			thread.join();

			loadStatus = LOAD_LOADED;
		}
	}

	auto GameResource::getLoaded() -> bool {
		return loadStatus != LOAD_UNLOADED;
	}

	auto GameResource::getDone() -> bool {
		return loadStatus == LOAD_DONE;
	}
}
