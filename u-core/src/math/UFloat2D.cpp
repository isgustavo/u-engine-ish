#include "UFloat2D.h"

namespace uei
{
	const bool uei::UFloat2D::operator == (const UFloat2D& b) const
	{
		return this->x == b.x && this->y == b.y;
	}

	UFloat2D uei::UFloat2D::operator + (const UFloat2D& b) const
	{
		return UFloat2D(this->x + b.x, this->y + b.y);
	}

	UFloat2D& uei::UFloat2D::operator += (const UFloat2D& b)
	{
		this->x += b.x;
		this->y += b.y;
		return *this;
	}

	UFloat2D uei::UFloat2D::operator * (float b) const
	{
		return UFloat2D(this->x * b, this->y * b);
	}
}