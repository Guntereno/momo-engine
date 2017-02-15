#ifndef MOMO_UTIL_STATEMACHINE_INCLUDED
#define MOMO_UTIL_STATEMACHINE_INCLUDED

#include "Momo.h"


namespace Momo
{
namespace Util
{

class StateMachine
{
public:
	StateMachine(u32 size = 64);
	~StateMachine();

	// When adding states StateMachine takes control of the memory for the sake of deletion
	void AddState(State* pState);

	void Begin(IdString state);
	void Update(const GameTime& gameTime);

	bool IsActive();

private:
	DISALLOW_COPY_AND_ASSIGN(StateMachine);

	State* FindState(IdString state);

	u32 mStateMax;
	State** mppStates;
	u32 mStateCount;

	State* mCurrentState;
};

}
}

#endif // MOMO_UTIL_STATEMACHINE_INCLUDED

