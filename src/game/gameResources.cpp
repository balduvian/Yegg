
#include "gameResources.h"

namespace Game {
	Rect GameResources::rect = Rect();
	TextureShader GameResources::textureShader = TextureShader();
	ColorShader GameResources::colorShader = ColorShader();
	CNGE::Texture GameResources::testTexture = CNGE::Texture("res/test.png");
	CNGE::Sound GameResources::testSound = CNGE::Sound("res/sound/test.wav");

	CNGE::ResourceBundle GameResources::loadScreenResources = CNGE::ResourceBundle({
		&rect, &colorShader
	});

	CNGE::ResourceBundle GameResources::gameResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader, &testSound
	});
}
