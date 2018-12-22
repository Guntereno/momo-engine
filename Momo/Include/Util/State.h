#ifndef MOMO_UTIL_STATE_INCLUDED
#define MOMO_UTIL_STATE_INCLUDED

#include "Momo.h"

#include "IdString.h"


namespace Momo
{
	namespace Util
	{

		class State
		{
		public:
			enum class Flags
			{
				Complete = 1 << 0,

				None = 0
			};

			State(IdString id);
			virtual ~State() {};

			Momo::IdString GetId() const { return mId; }

			void Enter();
			void Update(const GameTime& gameTime);
			void Exit();

			bool IsActive();
			Momo::IdString GetStateRequest() const { return mStateRequest; }

		protected:
			virtual void EnterInternal() {};
			virtual void UpdateInternal(const GameTime& gameTime) = 0;
			virtual void ExitInternal() {};

			// Use to request exit to another state
			void RequestState(IdString id);

		private:
			DISALLOW_COPY_AND_ASSIGN(State);

			IdString mId;

			Flags mFlags;
			IdString mStateRequest;
		};


		FLAG_OPS(State::Flags)
	}
}


#endif // MOMO_UTIL_STATE_INCLUDED
