#include "Vector.h"

namespace Momo
{

	Vector2 operator* (float scalar, const Vector2& vector)
	{
		Vector2 result = { scalar * vector.mX, scalar * vector.mY };
		return result;
	}

	Vector2 operator* (const Vector2& vector, float scalar)
	{
		Vector2 result = { scalar * vector.mX, scalar * vector.mY };
		return result;
	}

	Vector2 operator+ (const Vector2& vector1, const Vector2& vector2)
	{
		Vector2 result = { vector1.mX + vector2.mX, vector1.mY + vector2.mY };
		return result;
	}


	Vector3 operator* (float scalar, const Vector3& vector)
	{
		Vector3 result = { scalar * vector.mX, scalar * vector.mY };
		return result;
	}

	Vector3 operator* (const Vector3& vector, float scalar)
	{
		Vector3 result = { scalar * vector.mX, scalar * vector.mY };
		return result;
	}

	Vector3 operator+ (const Vector3& vector1, const Vector3& vector2)
	{
		Vector3 result = { vector1.mX + vector2.mX, vector1.mY + vector2.mY };
		return result;
	}

}