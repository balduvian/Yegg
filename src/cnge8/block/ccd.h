
#ifndef CNGE_CCD
#define CNGE_CCD

#include "../math/Vector2.h"
#include "../Types.h"

namespace CNGE {
	constexpr f32 CCD_SKEK = 0.0001_f32;

	struct BoundingBox {
		BoundingBox(Vector2f start, Vector2f end) :
			minX(floor(fmin(start.getX(), end.getX())) - 1),
			maxX(ceil(fmax(start.getX(), end.getX())) + 1),
			minY(floor(fmin(start.getY(), end.getY())) - 1),
			maxY(ceil(fmax(start.getY(), end.getY())) + 1) {}

		i32 minX, maxX, minY, maxY;
	};

	struct CollStruct {
		bool coll = false;
		f32 closestT = std::numeric_limits<f32>::max();
		f32 collPos = 0_f32;
	};

	f32 interp(f32 start, f32 end, f32 t);

	void horizontalColl(CollStruct& horizontalColl, const Vector2f start, const Vector2f end, f32 offX, f32 wallX, f32 wallDown, f32 wallUp);

	/// for quick and dirty jobs where the specifics don't matter
	/// you are given no information about the collision except if it happened
	bool horizontalTest(const Vector2f start, const Vector2f end, f32 wallX, f32 wallDown, f32 wallUp);

	void verticalColl(CollStruct& verticalColl, const Vector2f start, const Vector2f end, f32 offY, f32 wallY, f32 wallLeft, f32 wallRight);

	bool verticalTest(const Vector2f start, const Vector2f end, f32 wallY, f32 wallLeft, f32 wallRight);
}

#endif
