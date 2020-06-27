
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "../cnge8/engine/texture/texture.h"
#include "../cnge8/load/bundle/resourceBundle.h"
#include "audio/sound.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/textureShader.h"

namespace Game {
	class GameResources {
	public:
		static Rect rect;
		static TextureShader textureShader;
		static ColorShader colorShader;
		static CNGE::Texture testTexture;
		static CNGE::Sound testSound;

		static CNGE::ResourceBundle loadScreenResources;
		static CNGE::ResourceBundle gameResources;
	};
}

#endif
