#include <iostream>

#include "eggShader.h"

namespace Game {
	EggShader::EggShader() : Shader("res/shaders/egg/vert.glsl", "res/shaders/egg/frag.glsl") {};

	auto EggShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
	}

	auto EggShader::giveColor(const f32 r, const f32 g, const f32 b, const f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
	}

	auto EggShader::giveColor(const CNGE::Color& color) -> void {
		giveVector4(colorLoc, color.r, color.g, color.b, color.a);
	}
}
