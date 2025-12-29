#pragma once

namespace uei
{
	struct UFloat2D
	{
		float x;
		float y;

		UFloat2D() : x(.0f), y(.0f) {}
		UFloat2D(float inX, float inY) : x(inX), y(inY) {}

		const bool operator == (const UFloat2D& b) const;
		UFloat2D operator + (const UFloat2D& b) const;
		UFloat2D& operator += (const UFloat2D& b);
		UFloat2D operator * (float b) const;
		//distance
		//magnitude
		//normalized
	};
}