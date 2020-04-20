
#include "ccd.h"

namespace CNGE {
	f32 interp(f32 start, f32 end, f32 t) {
		return (end - start) * t + start;
	};

	void horizontalColl(CollStruct& horizontalColl, const Vector2f start, const Vector2f end, f32 offX, f32 wallX, f32 wallDown, f32 wallUp) {
		auto tValue = (wallX - start.getX()) / (end.getX() - start.getX());

		if (tValue >= 0 && tValue <= 1 && tValue < horizontalColl.closestT) {
			auto pValue = interp(start.getY(), end.getY(), tValue);

			if (pValue >= wallDown && pValue <= wallUp) {
				horizontalColl.coll = true;
				horizontalColl.closestT = tValue;
				horizontalColl.collPos = interp(start.getX(), end.getX(), tValue) - offX;
			}
		}
	}

	/// for quick and dirty jobs where the specifics don't matter
	/// you are given no information about the collision except if it happened
	bool horizontalTest(const Vector2f start, const Vector2f end, f32 wallX, f32 wallDown, f32 wallUp) {
		auto tValue = (wallX - start.getX()) / (end.getX() - start.getX());

		if (tValue >= 0 && tValue <= 1 && tValue) {
			auto pValue = interp(start.getY(), end.getY(), tValue);

			if (pValue >= wallDown && pValue <= wallUp)
				return true;
		}

		return false;
	}

	void verticalColl(CollStruct& verticalColl, const Vector2f start, const Vector2f end, f32 offY, f32 wallY, f32 wallLeft, f32 wallRight) {
		auto tValue = (wallY - start.getY()) / (end.getY() - start.getY());

		if (tValue >= 0 && tValue <= 1 && tValue < verticalColl.closestT) {
			auto pValue = interp(start.getX(), end.getX(), tValue);

			if (pValue >= wallLeft && pValue <= wallRight) {
				verticalColl.coll = true;
				verticalColl.closestT = tValue;
				verticalColl.collPos = interp(start.getY(), end.getY(), tValue) - offY;
			}
		}
	}

	bool verticalTest(const Vector2f start, const Vector2f end, f32 wallY, f32 wallLeft, f32 wallRight) {
		auto tValue = (wallY - start.getY()) / (end.getY() - start.getY());

		if (tValue >= 0 && tValue <= 1 && tValue) {
			auto pValue = interp(start.getX(), end.getX(), tValue);

			if (pValue >= wallLeft && pValue <= wallRight)
				return true;
		}

		return false;
	}

}
