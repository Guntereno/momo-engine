#ifndef MOMO_APPLICATIONTIMER_INCLUDED
#define MOMO_APPLICATIONTIMER_INCLUDED

#include "Momo.h"

namespace Momo
{
class ApplicationTimer
{
public:
	ApplicationTimer();

	void Init();

	void Update();
	u64 GetCurrentTick() { return mCurrentTick; }
	void SetPaused(bool value) { mPaused = value; }

private:
	u64 mPreviousTick;
	u64 mCurrentTick;
	bool mPaused;
};
}

#endif // MOMO_APPLICATIONTIMER_INCLUDED
