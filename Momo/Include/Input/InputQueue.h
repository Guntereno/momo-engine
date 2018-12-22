#ifndef MOMO_INPUT_INPUTQUEUE_INCLUDED
#define MOMO_INPUT_INPUTQUEUE_INCLUDED

#include "InputEvent.h"


namespace Momo
{
	namespace Input
	{

		// FIFO queue for tracking input events
		class InputQueue
		{
		public:
			static Event kNullEvent;

			InputQueue(size_t size = 256);
			~InputQueue();

			void Push(Event& event);
			const Event& Pop();

			size_t Count() const { return mCount; }

		private:
			size_t mSize;
			Event* mQueue;

			Event* mpStart;
			size_t mCount;
		};

	}
}

#endif //MOMO_INPUT_INPUTQUEUE_INCLUDED
