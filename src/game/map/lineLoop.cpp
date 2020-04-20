
#include "lineLoop.h"

namespace Game {
	/// <summary>
	///	sets internal values 
	/// called from other constructors
	/// </summary>
	LineLoop::LineLoop(i32 size, bool connected)
		/* the number of lines = the number of points in a connected loop */
		: length(connected ? size : size - 1),

		lines(new CCDLine[length]),
		connected(connected)
	{}

	/// <summary>
	///	constructs lines with a dynamic array of points
	/// </summary>
	LineLoop::LineLoop(i32 numPoints, Vector2f* points, bool connected)
		: LineLoop(numPoints, connected)
	{
		init(points, points + numPoints);
	}

	/// <summary>
	///	constructs lines with a static list of points
	/// </summary>
	LineLoop::LineLoop(std::initializer_list<Vector2f> points, bool connected)
		: LineLoop(points.size(), connected)
	{
		init(points.begin(), points.end());
	}

	auto LineLoop::init(const Vector2f* begin, const Vector2f* end) -> void {
		/* construct the lines from the points */
		auto lastPoint = begin;
		auto second = begin + 1;

		for (auto i = second; i < end; ++i) {
			lines[i - second].set(lastPoint, i);

			lastPoint = i;
		}

		/* connect the last point back to the first */
		if (connected)
			lines[length - 1].set(lastPoint, begin);
	}

	LineLoop::LineLoop(const LineLoop& other)
		: length(other.length), lines(new CCDLine[length]), connected(other.connected) {
		
		for (auto i = 0; i < length; ++i)
			lines[i] = other.lines[i];
	}

	LineLoop::LineLoop(LineLoop&& other) noexcept
		: length(other.length), lines(other.lines), connected(other.connected) {
		other.lines = nullptr;
	}

	LineLoop::~LineLoop() {
		if (lines)
			delete[] lines;
	}

	auto LineLoop::begin() -> CCDLine* {
		return lines;
	}

	auto LineLoop::end() -> CCDLine* {
		return lines + length;
	}

	auto LineLoop::getConnected() -> bool {
		return connected;
	}

	auto LineLoop::get(i32 index) -> CCDLine* {
		return &lines[index];
	}
}
