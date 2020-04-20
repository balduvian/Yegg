
#ifndef GAME_GAME_RESOURCE
#define GAME_GAME_RESOURCE

#include <thread>
#include <mutex> 
#include "types.h"

namespace Game {
	class GameResource {
	private:
		constexpr static i32 LOAD_UNLOADED = 0;
		constexpr static i32 LOAD_DONE = 1;
		constexpr static i32 LOAD_LOADED = 2;

		static std::mutex mutex;

		std::thread thread;

		i32 loadStatus;

	protected:
		virtual auto customLoad() -> void = 0;
		virtual auto customUnload() -> void = 0;

	public:
		GameResource();

		auto hardLoad() -> void;

		auto load() -> void;
		auto loadUpdate() -> void;
		auto unload() -> void;

		auto getLoaded() -> bool;
		auto getDone() -> bool;
	};
}

#endif
