#pragma once

#ifndef GAME_CIRCLESHADER
#define GAME_CIRCLESHADER

#include "../../cnge8/engine/shader.h"
#include "../../cnge8/Types.h"
#include "../../cnge8/util/color.h"

namespace Game {
	class CircleShader : public CNGE::Shader {
	private:
		i32 colorLoc;

	public:
		CircleShader();

		auto getUniforms() -> void override;

		auto giveColor(const f32, const f32, const f32, const f32) -> void;

		auto giveColor(const CNGE::Color& color) -> void;
	};
}

#endif