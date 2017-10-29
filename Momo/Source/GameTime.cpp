#include "GameTime.h"

namespace Momo
{

	GameTime::GameTime() :
		mCurrentTick(0),
		mPreviousTick(0)
	{
	}

	GameTime& GameTime::operator= (const GameTime& value)
	{
		mCurrentTick = value.mCurrentTick;
		mPreviousTick = value.mPreviousTick;
		return *this;
	}

	void GameTime::Update(u64 tick)
	{
		// Allow timer to reset
		if (tick < mPreviousTick)
		{
			mPreviousTick = mCurrentTick = tick;
		}
		else
		{
			mPreviousTick = mCurrentTick;
			mCurrentTick = tick;
		}
	}

	u64 GameTime::GetDeltaMilliseconds() const
	{
		return (mCurrentTick - mPreviousTick);
	}

	float GameTime::GetDeltaSeconds() const
	{
		return MillisecondsToSeconds(GetDeltaMilliseconds());
	}

	u64 GameTime::SecondsToMilliseconds(float value)
	{
		return (u64)(value * 1000.0f);
	}

	float GameTime::MillisecondsToSeconds(u64 value)
	{
		return ((float)value) / 1000.0f;
	}

	Momo::GameTime GameTime::WithoutDelta() const
	{
		GameTime newTime = *this;
		newTime.mPreviousTick = newTime.mCurrentTick;

		return newTime;
	}

}
