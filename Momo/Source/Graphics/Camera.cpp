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
			mProjection.SetOrthographicProjection((float)viewport.width, (float)viewport.height, -1.0f, 1.0f);
			mViewProjectionDirty = true;
		}

		void Camera::RecalculateViewProjection() const
		{
			Matrix::Multiply(mProjection, mView, mViewProjection);
			mViewProjectionDirty = false;
		}


	}
}