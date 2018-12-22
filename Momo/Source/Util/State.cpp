#include "Util/State.h"

#include "GameTime.h"


namespace Momo
{
	namespace Util
	{
		State::State(IdString id) :
			mId(id),
			mFlags(0),
			mStateRequest()
		{
			// Nothing
		}

		void State::Enter()
		{
			mFlags = 0;
			mStateRequest = IdString();

			EnterInternal();
		}

		void State::Update(const GameTime& gameTime)
		{
			UpdateInternal(gameTime);
		}

		void State::Exit()
		{
			ExitInternal();
		}

		bool State::IsActive()
		{
			return ((mFlags & kFlagComplete) != 0);
		}

		void State::RequestState(IdString id)
		{
			mStateRequest = id;
			mFlags |= kFlagComplete;
		}
	}
}

