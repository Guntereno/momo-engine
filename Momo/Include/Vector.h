#ifndef MOMO_VECTOR_INCLUDED
#define MOMO_VECTOR_INCLUDED

#include "Momo.h"

#include "Point.h"

namespace Momo
{
// POD type representing a 2D Vector
struct Vector2
{
	float x;
	float y;

	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	
	void Set(const Point& point)
	{
		this->x = (float)(point.x);
		this->y = (float)(point.y);
	}

	static Vector2 Zero()
	{
		Vector2 zero;
		zero.Set(0.0f, 0.0f);
		return zero;
	}

	bool operator==(const Vector2& rhs)
	{
		return
			(this->x == rhs.x) &&
			(this->y == rhs.y);
	}

	bool operator!=(const Vector2& rhs)
	{
		return
			(this->x != rhs.x) ||
			(this->y != rhs.y);
	}

	Vector2& operator+=(const Vector2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

Vector2 operator* (float scalar, const Vector2& vector);
Vector2 operator* (const Vector2& vector, float scalar);
Vector2 operator+ (const Vector2& vector1, const Vector2& vector2);

// POD type representing a 4D Vector
struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	static Vector4 Zero()
	{
		Vector4 zero;
		zero.Set(0.0f, 0.0f, 0.0f, 0.0f);
		return zero;
	}

	bool operator==(const Vector4 &rhs)
	{
		return
			(this->x == rhs.x) &&
			(this->y == rhs.y) &&
			(this->z == rhs.z) &&
			(this->w == rhs.w);
	}

	bool operator!=(const Vector4 &rhs)
	{
		return
			(this->x != rhs.x) ||
			(this->y != rhs.y) ||
			(this->z != rhs.z) ||
			(this->w != rhs.w);
	}
};
}

#endif //MOMO_VECTOR_INCLUDED
