#include "Util/StateMachine.h"

#include <cstring>

#include "GameTime.h"
#include "Util/State.h"


namespace Momo
{
	namespace Util
	{

		StateMachine::StateMachine(const u32 size /*= 64*/) :
			mStateMax(size),
			mppStates(NULL),
			mCurrentState(NULL)
		{
			if (mStateMax > 0)
			{
				mppStates = new State*[mStateMax];
				memset(mppStates, 0, sizeof(mppStates));
			}
		}

		StateMachine::~StateMachine()
		{
			for (u32 i = 0; i < mStateCount; ++i)
			{
				delete mppStates[i];
				mppStates[i] = NULL;
			}

			delete[] mppStates;
		}

		void StateMachine::AddState(State* pState)
		{
			ASSERT(pState != NULL);

			if (mStateCount < mStateMax)
			{
				mppStates[mStateCount++] = pState;
			}
			else
			{
				BREAK_MSG("State exceeded buffer bounds!");
			}
		}

		void StateMachine::Begin(IdString state)
		{
			State* pState = FindState(state);
			if (pState == NULL)
			{
				BREAK_MSG("Unable to find specified state!");
			}
			else
			{
				mCurrentState = pState;
				mCurrentState->Enter();
			}
		}

		void StateMachine::Update(const GameTime& gameTime)
		{
			if (mCurrentState != NULL)
			{
				mCurrentState->Update(gameTime);

				if (!mCurrentState->IsActive())
				{
					mCurrentState->Exit();

					IdString request = mCurrentState->GetStateRequest();
					if (request != IdString())
					{
						State* pState = FindState(request);
						if (pState == NULL)
						{
							mCurrentState = NULL;
							BREAK_MSG("Unable to find specified state!");
						}
						else
						{
							mCurrentState = pState;
							mCurrentState->Enter();

							// To ensure states can fall through call recursively with no delta
							GameTime noDelta = gameTime.WithoutDelta();
							Update(noDelta);
						}
					}
					else
					{
						mCurrentState = NULL;
					}
				}
			}
		}

		bool StateMachine::IsActive()
		{
			return mCurrentState != NULL;
		}

		State* StateMachine::FindState(IdString state)
		{
			for (u32 i = 0; i < mStateCount; ++i)
			{
				if (state == mppStates[i]->GetId())
				{
					return mppStates[i];
				}
			}

			return NULL;
		}

	}
}

