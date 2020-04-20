
#ifndef GAME_COLORSHADER
#define GAME_COLORSHADER

#include "../../cnge8/engine/shader.h"
#include "../../cnge8/Types.h"
#include "../../cnge8/util/color.h"

namespace Game {
	class ColorShader : public CNGE::Shader {
	private:
		i32 colorLoc;

	public:
		ColorShader();

		auto getUniforms() -> void override;

		auto giveColor(const f32, const f32, const f32, const f32) -> void;

		auto giveColor(const CNGE::Color &color) -> void;
	};
}

#endif