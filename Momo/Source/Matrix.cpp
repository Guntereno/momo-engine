#include "Matrix.h"

#include "Vector.h"

#include <string.h>


namespace Momo
{
void Matrix::SetIdentity()
{
    memset(this, 0, sizeof(Matrix));

    m11 = 1.0f;
    m22 = 1.0f;
    m33 = 1.0f;
    m44 = 1.0f;
}

void Matrix::SetOrthographicProjection(float width, float height, float zNearPlane, float zFarPlane)
{
    memset(this, 0, sizeof(Matrix));

    m11 = 2.0f / width;
    m22 = 2.0f / height;
    m33 = 1.0f / (zNearPlane - zFarPlane);
    m43 = zNearPlane / (zNearPlane - zFarPlane);
    m44 = 1.0f;
}

void Matrix::SetTranslation(const Vector3& pos)
{
    memset(this, 0, sizeof(Matrix));

    m11 = 1.0f;
    m22 = 1.0f;
    m33 = 1.0f;

    m14 = pos.mX;
    m24 = pos.mY;
    m34 = pos.mZ;
    m44 = 1.0f;
}

void Matrix::SetTranslation(float x, float y, float z)
{
    memset(this, 0, sizeof(Matrix));

    m11 = 1.0f;
    m22 = 1.0f;
    m33 = 1.0f;

    m14 = x;
    m24 = y;
    m34 = z;
    m44 = 1.0f;
}

void Matrix::Multiply(const Matrix& matrix1, const Matrix& matrix2, Matrix& result)
{
    float m = matrix1.m11 * matrix2.m11 + matrix1.m12 * matrix2.m21 + matrix1.m13 * matrix2.m31 + matrix1.m14 * matrix2.m41;
    float m2 = matrix1.m11 * matrix2.m12 + matrix1.m12 * matrix2.m22 + matrix1.m13 * matrix2.m32 + matrix1.m14 * matrix2.m42;
    float m3 = matrix1.m11 * matrix2.m13 + matrix1.m12 * matrix2.m23 + matrix1.m13 * matrix2.m33 + matrix1.m14 * matrix2.m43;
    float m4 = matrix1.m11 * matrix2.m14 + matrix1.m12 * matrix2.m24 + matrix1.m13 * matrix2.m34 + matrix1.m14 * matrix2.m44;
    float m5 = matrix1.m21 * matrix2.m11 + matrix1.m22 * matrix2.m21 + matrix1.m23 * matrix2.m31 + matrix1.m24 * matrix2.m41;
    float m6 = matrix1.m21 * matrix2.m12 + matrix1.m22 * matrix2.m22 + matrix1.m23 * matrix2.m32 + matrix1.m24 * matrix2.m42;
    float m7 = matrix1.m21 * matrix2.m13 + matrix1.m22 * matrix2.m23 + matrix1.m23 * matrix2.m33 + matrix1.m24 * matrix2.m43;
    float m8 = matrix1.m21 * matrix2.m14 + matrix1.m22 * matrix2.m24 + matrix1.m23 * matrix2.m34 + matrix1.m24 * matrix2.m44;
    float m9 = matrix1.m31 * matrix2.m11 + matrix1.m32 * matrix2.m21 + matrix1.m33 * matrix2.m31 + matrix1.m34 * matrix2.m41;
    float m10 = matrix1.m31 * matrix2.m12 + matrix1.m32 * matrix2.m22 + matrix1.m33 * matrix2.m32 + matrix1.m34 * matrix2.m42;
    float m11 = matrix1.m31 * matrix2.m13 + matrix1.m32 * matrix2.m23 + matrix1.m33 * matrix2.m33 + matrix1.m34 * matrix2.m43;
    float m12 = matrix1.m31 * matrix2.m14 + matrix1.m32 * matrix2.m24 + matrix1.m33 * matrix2.m34 + matrix1.m34 * matrix2.m44;
    float m13 = matrix1.m41 * matrix2.m11 + matrix1.m42 * matrix2.m21 + matrix1.m43 * matrix2.m31 + matrix1.m44 * matrix2.m41;
    float m14 = matrix1.m41 * matrix2.m12 + matrix1.m42 * matrix2.m22 + matrix1.m43 * matrix2.m32 + matrix1.m44 * matrix2.m42;
    float m15 = matrix1.m41 * matrix2.m13 + matrix1.m42 * matrix2.m23 + matrix1.m43 * matrix2.m33 + matrix1.m44 * matrix2.m43;
    float m16 = matrix1.m41 * matrix2.m14 + matrix1.m42 * matrix2.m24 + matrix1.m43 * matrix2.m34 + matrix1.m44 * matrix2.m44;

    result.m11 = m;
    result.m12 = m2;
    result.m13 = m3;
    result.m14 = m4;
    result.m21 = m5;
    result.m22 = m6;
    result.m23 = m7;
    result.m24 = m8;
    result.m31 = m9;
    result.m32 = m10;
    result.m33 = m11;
    result.m34 = m12;
    result.m41 = m13;
    result.m42 = m14;
    result.m43 = m15;
    result.m44 = m16;
}

Matrix Matrix::Identity()
{
    Matrix matrix;
    matrix.SetIdentity();
    return matrix;
}

Matrix Matrix::Translation(float x, float y, float z)
{
    Matrix matrix;
    matrix.SetTranslation(x, y, z);
    return matrix;
}

Matrix Matrix::Translation(const Vector3& pos)
{
    Matrix matrix;
    matrix.SetTranslation(pos);
    return matrix;
}

Matrix operator* (const Matrix& a, const Matrix& b)
{
    Matrix result;
    Matrix::Multiply(a, b, result);
    return result;
}
}