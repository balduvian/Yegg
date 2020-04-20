
#include "CCDLine.h"

namespace Game {
	CCDLine::CCDLine() {}

	CCDLine::CCDLine(f32 x0, f32 y0, f32 x1, f32 y1)
		: start(x0, y0), end(x1, y1) {}
	
	CCDLine::CCDLine(const Vector2f& start, const Vector2f&& end)
		: start(start), end(end) {}

	CCDLine::CCDLine(const Vector2f&& start, const Vector2f&& end)
		: start(start), end(end) {}

	CCDLine::CCDLine(const Vector2f* start, const Vector2f* end)
		: start(*start), end(*end) {}

	CCDLine::CCDLine(const CCDLine* origin, i32 x, i32 y)
		: start(origin->start), end(origin->end) {
		start.translateXY(x, y);
		end.translateXY(x, y);
	}

	auto operator<<(std::ostream& stream, const CCDLine& line) -> std::ostream& {
		stream << '<'<< line.start << ", " << line.end << '>';

		return stream;
	}

	auto CCDLine::set(f32 x0, f32 y0, f32 x1, f32 y1) -> void{
		start.setXY(x0, y0);
		end.setXY(x1, y1);
	}

	auto CCDLine::set(const Vector2f* start, const Vector2f* end) -> void {
		this->start.set(*start);
		this->end.set(*end);
	}

	auto CCDLine::toVector() -> Vector2f {
		return (end - start);
	}

	auto CCDLine::operator+(const Vector2f& vector) -> CCDLine {
		return CCDLine(start + vector, end + vector);
	}

	auto CCDLine::operator-(const Vector2f& vector) -> CCDLine {
		return CCDLine(start - vector, end - vector);
	}

	auto CCDLine::operator*(f32 amount) -> CCDLine {
		return CCDLine(start * amount, end * amount);
	}

	auto CCDLine::setEnd(const Vector2f& newEnd) -> void {
		end = newEnd;
	}
}