#include "Vector.h"

namespace Momo
{

	Vector2 operator* (float scalar, const Vector2& vector)
	{
		Vector2 result = { scalar * vector.x, scalar * vector.y };
		return result;
	}

	Vector2 operator* (const Vector2& vector, float scalar)
	{
		Vector2 result = { scalar * vector.x, scalar * vector.y };
		return result;
	}

	Vector2 operator+ (const Vector2& vector1, const Vector2& vector2)
	{
		Vector2 result = { vector1.x + vector2.x, vector1.y + vector2.y };
		return result;
	}


	Vector3 operator* (float scalar, const Vector3& vector)
	{
		Vector3 result = { scalar * vector.x, scalar * vector.y };
		return result;
	}

	Vector3 operator* (const Vector3& vector, float scalar)
	{
		Vector3 result = { scalar * vector.x, scalar * vector.y };
		return result;
	}

	Vector3 operator+ (const Vector3& vector1, const Vector3& vector2)
	{
		Vector3 result = { vector1.x + vector2.x, vector1.y + vector2.y };
		return result;
	}

}