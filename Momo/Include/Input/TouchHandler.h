#ifndef MOMO_INPUT_TOUCHHANDLER_INCLUDED
#define MOMO_INPUT_TOUCHHANDLER_INCLUDED

#include "Momo.h"

#include "Point.h"


namespace Momo
{
	namespace Input
	{

		class TouchHandler
		{
		public:
			static const int kMaxTouches = 5;

			TouchHandler();

			int GetNumTouches() const { return mNumTouches; }
			const Point& GetTouch(int index) const { return mTouches[index]; }

		private:
			Point mTouches[kMaxTouches];
			int mNumTouches;
		};

	}
}

#endif //MOMO_INPUT_TOUCHHANDLER_INCLUDED
