#include "Input/InputQueue.h"

#include "AssertBreak.h"

#include <cstring>
#include <cstdlib>


namespace Momo
{
	namespace Input
	{

		Event InputQueue::kNullEvent =
		{
			Event::kNone,
			(Event::Id) - 1,
			Point::Zero(),
			Point::Zero()
		};

		InputQueue::InputQueue(size_t size) :
			mSize(size),
			mQueue(NULL),
			mpStart(NULL),
			mCount(0)
		{
			mQueue = static_cast<Event*>(malloc(sizeof(Event) * mSize));
			memset(mQueue, 0, sizeof(Event) * mSize);
		}

		InputQueue::~InputQueue()
		{
			free(mQueue);
		}

		void InputQueue::Push(Event& event)
		{
			if (mCount >= mSize)
			{
				BREAK_MSG("Exceeded size of InputQueue (%d)!", mSize);
				return;
			}

			if (mpStart == NULL)
			{
				mpStart = mQueue;
			}
			else
			{
				++mpStart;
				size_t offset = (size_t)(mpStart - mQueue);
				if (offset >= mSize)
				{
					mpStart = mQueue;
				}
			}

			++mCount;

			memcpy(mpStart, &event, sizeof(Event));
		}

		const Event& InputQueue::Pop()
		{
			if (mCount == 0)
			{
				ASSERT("Attempting to Pop empty input queue!");
				return kNullEvent;
			}

			Event& event = *mpStart;

			++mpStart;
			--mCount;

			return event;
		}


	}
}

