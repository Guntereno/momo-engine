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

	InputQueue(int size = 256);
	~InputQueue();

	void Push(Event& event);
	const Event& Pop();

	short Count() const { return mCount; }

private:
	int mSize;
	Event* mQueue;

	Event* mpStart;
	int mCount;
};

}
}

#endif //MOMO_INPUT_INPUTQUEUE_INCLUDED
