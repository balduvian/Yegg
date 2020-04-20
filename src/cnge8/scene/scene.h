
#ifndef CNGE_SCENE
#define CNGE_SCENE

#include "../Types.h"
#include "../load/resource.h"
#include "../engine/loop.h"
#include "../engine/window.h"
#include "../load/bundle/ResourceBundle.h"

namespace CNGE {
	class Scene {
	private:
        ResourceBundle* bundle;

        bool switching;
        i32 switchCode;

	protected:
		void switchScene(i32);

	public:
        Scene(ResourceBundle* bundle);

        virtual void start() = 0;

        virtual void resized(u32 width, u32 height) = 0;

        virtual void update(Input* input, Timing* time) = 0;

        virtual void render() = 0;

        /// scene switching ///

		bool getSwitching();
        int getSwitchCode();

        auto getBundle() -> ResourceBundle*;
	};
}

#endif
