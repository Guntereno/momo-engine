#include "Graphics/Camera.h"


namespace Momo
{
	namespace Graphics
	{

		Camera::Camera()
		{

		}

		void Camera::SetView(const Matrix& view)
		{
			mView = view;
			mViewProjectionDirty = true;
		}

		void Camera::SetProjectionOrtho(const Rectangle& viewport, float near, float far)
		{
			mProjection.SetOrthographicProjection((float)viewport.mWidth, (float)viewport.mHeight, near, far);
			mViewProjectionDirty = true;
		}

		void Camera::RecalculateViewProjection() const
		{
			Matrix::Multiply(mProjection, mView, mViewProjection);
			mViewProjectionDirty = false;
		}


	}
}
