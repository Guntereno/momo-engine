#ifndef MOMO_GRAPHICS_CAMERA_INCLUDED
#define MOMO_GRAPHICS_CAMERA_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Matrix.h"
#include "Rectangle.h"
#include "Vector.h"

#include "Graphics/Texture.h"

namespace Momo
{
	namespace Graphics
	{
		class Camera
		{
		public:
			Camera();

			void SetView(const Matrix& view);
			void SetProjectionOrtho(const Rectangle& viewport, float near, float far);

			inline const Matrix& GetView() const { return mView; }
			inline const Matrix& GetProjection() const { return mProjection; }
			inline const Matrix& GetViewProjection() const
			{
				if (mViewProjectionDirty)
				{
					RecalculateViewProjection();
				}
				return mViewProjection;
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(Camera);

			void RecalculateViewProjection() const;

			Matrix mView;
			Matrix mProjection;

			mutable bool mViewProjectionDirty;
			mutable Matrix mViewProjection;
		};
	}
}

#endif //MOMO_GRAPHICS_CAMERA_INCLUDED
