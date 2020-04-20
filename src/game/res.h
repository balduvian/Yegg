
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "../cnge8/engine/texture/texture.h"
#include "../cnge8/load/bundle/resourceBundle.h"
#include "engine/texture/tileGrid.h"
#include "audio/sound.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/circleShader.h"
#include "graphics/textureShader.h"
#include "graphics/eggShader.h"

namespace Game {
	class Res {
	public:
		static Rect rect;
		static EggShape eggShape;
		static Triangle triangle;
		static EggshellShape eggshellShape;
		static LauncherShape launcherShape;

		static TextureShader textureShader;
		static ColorShader colorShader;
		static CircleShader circleShader;
		static EggShader eggShader;
		
		static CNGE::Sound deathSound;
		static CNGE::Sound songSound;

		static CNGE::TileGrid blockTexture;
		static CNGE::Texture eggTexture;
		static CNGE::Texture testTexture;
		static CNGE::Texture ropeTexture;

		static CNGE::ResourceBundle loadScreenResources;
		static CNGE::ResourceBundle gameResources;
	};
}

#endif
