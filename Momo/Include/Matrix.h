#ifndef MOMO_MATRIX_INCLUDED
#define MOMO_MATRIX_INCLUDED

#include "Momo.h"


namespace Momo
{

	// POD Type representing a 4x4 Matrix
	struct Matrix
	{
		float m11;
		float m21;
		float m31;
		float m41;

		float m12;
		float m22;
		float m32;
		float m42;

		float m13;
		float m23;
		float m33;
		float m43;

		float m14;
		float m24;
		float m34;
		float m44;

		void SetIdentity();

		void SetOrthographicProjection(float width, float height, float zNearPlane, float zFarPlane);
		void SetPerspectiveFieldOfView(float fieldOfViewRads, float aspectRatio, float zNearPlane, float zFarPlane);

		void SetTranslation(const Vector3& pos);
		void SetTranslation(float x, float y, float z);

		static void Multiply(const Matrix& matrix1, const Matrix& matrix2, Matrix& result);

		static Matrix Identity();

		static Matrix Translation(float x, float y, float z);
		static Matrix Translation(const Vector3& pos);
	};

	Matrix operator* (const Matrix& a, const Matrix& b);

	POD_CHECK(Matrix);
}

#endif //MOMO_MATRIX_INCLUDED
