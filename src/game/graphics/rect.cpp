
#include <GL/glew.h>

#include "rect.h"
#include "../../cnge8/types.h"

namespace Game {
	Rect::Rect() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(12, new f32[12] {
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
		}),
		6, new u32[6] {
			0, 3, 2,
			0, 2, 1
		},
		1, new CNGE::Attribute[1] {
			CNGE::Attribute(2, 8, new f32[8] {
				0, 1,
				0, 0,
				1, 0,
				1, 1
			})
		}
	) {}

	CardRect::CardRect() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(12, new f32[12] {
			-.5, 0, 0,
			.5, 0, 0,
			.5, 1, 0,
			-.5, 1, 0
		}),
		6, new u32[6] {
			0, 1, 2,
			0, 2, 3
		},
		1, new CNGE::Attribute[1] {
			CNGE::Attribute(2, 8, new f32[8] {
				0, 1,
				1, 1,
				1, 0,
				0, 0
			})
		}
	) {}

	EggShape::EggShape() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(27, new f32[27] {
			0, 0, 0,
			0, 15, 0,
			7, 10, 0,
			11, -3, 0,
			7, -12, 0,
			0, -15, 0,
			-7, -12, 0,
			-11, -3, 0,
			-7, 10, 0
			}),
		24, new u32[24] {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 1
		},
		1, new CNGE::Attribute[1]{
			CNGE::Attribute(1, 9, new f32[9] {
				1,
				0.86,
				0.86,
				0.86,
				0.86,
				0.86,
				0.86,
				0.86,
				0.86,
			})
		}
	) {}

	Triangle::Triangle() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(9, new f32[9]{
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
			}),
		3, new u32[3]{
			0, 1, 2
		},
		0, nullptr
	) {}

	EggshellShape::EggshellShape() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(12, new f32[12]{
			0, 0, 0,
			0.5f, -0.288633f, 0,
			0, 0.577344f, 0,
			-0.5, -0.288633f, 0,
			}),
		9, new u32[9]{
			0, 1, 2,
			0, 2, 3,
			0, 3, 1,
		},
		1, new CNGE::Attribute[1]{
			CNGE::Attribute(1, 4, new f32[4] {
				1,
				0.86,
				0.86,
				0.86,
			})
		}
	) {}

	LauncherShape::LauncherShape() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(24, new f32[24]{
			0, 0, 0,
			4/6.f, 0, 0,
			0, 6/6.f, 0,
			4/6.f, 6/6.f, 0,

			6/6.f, 2/6.f, 0,
			4/6.f, 2/6.f, 0,

			6/6.f, 4/6.f, 0,
			4/6.f, 4/6.f, 0
			}),
		12, new u32[12]{
			0, 1, 2,
			1, 2, 3,

			1, 4, 5,

			3, 6, 7
		},
		0, nullptr
	) {}
}
