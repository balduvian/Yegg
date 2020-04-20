
#include "res.h"

namespace Game {
	Rect Res::rect = Rect();
	EggShape Res::eggShape = EggShape();
	Triangle Res::triangle = Triangle();
	LauncherShape Res::launcherShape = LauncherShape();

	EggshellShape Res::eggshellShape = EggshellShape();
	TextureShader Res::textureShader = TextureShader();
	ColorShader Res::colorShader = ColorShader();
	CircleShader Res::circleShader = CircleShader();
	EggShader Res::eggShader = EggShader();

	CNGE::Sound Res::deathSound = CNGE::Sound("res/sound/death.wav");
	CNGE::Sound Res::songSound = CNGE::Sound("res/sound/song.wav");

	CNGE::Texture Res::eggTexture = CNGE::Texture("res/texture/egg.png");
	CNGE::Texture Res::testTexture = CNGE::Texture("res/texture/test.png");
	CNGE::TileGrid Res::blockTexture = CNGE::TileGrid("res/texture/block.png", 3, 2);
	CNGE::Texture Res::ropeTexture = CNGE::Texture("res/texture/rope.png");

	CNGE::ResourceBundle Res::loadScreenResources = CNGE::ResourceBundle({
		&rect, &colorShader
	});

	CNGE::ResourceBundle Res::gameResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader, &songSound, &eggTexture, &blockTexture, &circleShader, &ropeTexture, &eggShader, &eggShape, &triangle, &eggshellShape, &deathSound, &launcherShape
	});
}
