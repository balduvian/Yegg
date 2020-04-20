#include <iostream>

#include "circleShader.h"

namespace Game {
	CircleShader::CircleShader() : Shader("res/shaders/circle/vert.glsl", "res/shaders/circle/frag.glsl") {};

	auto CircleShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
	}

	auto CircleShader::giveColor(const f32 r, const f32 g, const f32 b, const f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
	}

	auto CircleShader::giveColor(const CNGE::Color& color) -> void {
		giveVector4(colorLoc, color.r, color.g, color.b, color.a);
	}
}
