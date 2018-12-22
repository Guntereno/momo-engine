#include "ApplicationTimer.h"
#include "Logger.h"

#ifdef WIN32
#include <windows.h>
#elif ANDROID
#include <sys/time.h>
#endif

#include <limits>

namespace Momo
{
	static constexpr u64 kTickInvalid = std::numeric_limits<u64>::max();

	static u64 GetTime()
	{
#ifdef ANDROID
		struct timeval now;
		gettimeofday(&now, NULL);
		return (long)(now.tv_sec * 1000 + now.tv_usec / 1000);
#elif WIN32
		return GetTickCount64();
#endif
	}

	ApplicationTimer::ApplicationTimer()
	{
		Init();
	}

	void ApplicationTimer::Init()
	{
		mPreviousTick = kTickInvalid;
		mCurrentTick = 0;
		mPaused = false;
	}

	void ApplicationTimer::Update()
	{
		u64 tick = GetTime();

		if (mPreviousTick != kTickInvalid)
		{
			u64 delta = tick - mPreviousTick;

			if (!mPaused)
			{
				mCurrentTick += delta;
			}
		}

		mPreviousTick = tick;
	}

}