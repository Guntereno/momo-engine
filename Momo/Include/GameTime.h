#ifndef MOMO_GAMETIME_INCLUDED
#define MOMO_GAMETIME_INCLUDED

#include "Momo.h"

namespace Momo
{
class GameTime
{
public:
	static u64 SecondsToMilliseconds(float value);
	static float MillisecondsToSeconds(u64 value);

	GameTime();

	GameTime& operator= (const GameTime& value);

	void Update(u64 tick);

	u64 GetDeltaMilliseconds() const;
	float GetDeltaSeconds() const;

	u64 GetCurrentTick() const { return mCurrentTick; }
	u64 GetPreviousTick() const { return mPreviousTick; }

	GameTime WithoutDelta() const;

private:
	u64 mCurrentTick;
	u64 mPreviousTick;
};
}

#endif // MOMO_GAMETIME_INCLUDED
