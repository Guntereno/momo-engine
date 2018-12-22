#ifndef MOMO_VECTOR_INCLUDED
#define MOMO_VECTOR_INCLUDED

#include "Momo.h"

#include "Point.h"


namespace Momo
{

	// POD type representing a 2D Vector
	struct Vector2
	{
		float mX;
		float mY;

		void Set(float x, float y)
		{
			this->mX = x;
			this->mY = y;
		}


		void Set(const Point& point)
		{
			this->mX = (float)(point.mX);
			this->mY = (float)(point.mY);
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
				(this->mX == rhs.mX) &&
				(this->mY == rhs.mY);
		}

		bool operator!=(const Vector2& rhs)
		{
			return
				(this->mX != rhs.mX) ||
				(this->mY != rhs.mY);
		}

		Vector2& operator+=(const Vector2& rhs)
		{
			this->mX += rhs.mX;
			this->mY += rhs.mY;
			return *this;
		}
	};

	Vector2 operator* (float scalar, const Vector2& vector);
	Vector2 operator* (const Vector2& vector, float scalar);
	Vector2 operator+ (const Vector2& vector1, const Vector2& vector2);

	POD_CHECK(Vector2);


	// POD type representing a 3D Vector
	struct Vector3
	{
		float mX;
		float mY;
		float mZ;

		void Set(float x, float y, float z)
		{
			this->mX = x;
			this->mY = y;
			this->mZ = z;
		}


		void Set(const Point& point)
		{
			this->mX = (float)(point.mX);
			this->mY = (float)(point.mY);
			this->mZ = 0.0f;
		}

		static Vector3 Zero()
		{
			Vector3 zero;
			zero.Set(0.0f, 0.0f, 0.0f);
			return zero;
		}

		bool operator==(const Vector3& rhs)
		{
			return
				(this->mX == rhs.mX) &&
				(this->mY == rhs.mY) &&
				(this->mZ == rhs.mZ);
		}

		bool operator!=(const Vector3& rhs)
		{
			return
				(this->mX != rhs.mX) ||
				(this->mY != rhs.mY) ||
				(this->mZ != rhs.mZ);
		}

		Vector3& operator+=(const Vector3& rhs)
		{
			this->mX += rhs.mX;
			this->mY += rhs.mY;
			this->mZ += rhs.mZ;
			return *this;
		}
	};

	Vector3 operator* (float scalar, const Vector3& vector);
	Vector3 operator* (const Vector3& vector, float scalar);
	Vector3 operator+ (const Vector3& vector1, const Vector3& vector2);

	POD_CHECK(Vector3);


	// POD type representing a 4D Vector
	struct Vector4
	{
		float mX;
		float mY;
		float mZ;
		float mW;

		void Set(float x, float y, float z, float w)
		{
			this->mX = x;
			this->mY = y;
			this->mZ = z;
			this->mW = w;
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
				(this->mX == rhs.mX) &&
				(this->mY == rhs.mY) &&
				(this->mZ == rhs.mZ) &&
				(this->mW == rhs.mW);
		}

		bool operator!=(const Vector4 &rhs)
		{
			return
				(this->mX != rhs.mX) ||
				(this->mY != rhs.mY) ||
				(this->mZ != rhs.mZ) ||
				(this->mW != rhs.mW);
		}
	};


	POD_CHECK(Vector4);
}

#endif //MOMO_VECTOR_INCLUDED
