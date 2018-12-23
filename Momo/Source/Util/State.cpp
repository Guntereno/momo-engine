#include "Util/State.h"

#include "GameTime.h"


namespace Momo
{
namespace Util
{
State::State(IdString id) :
    mId(id),
    mFlags(Flags::None),
    mStateRequest()
{
    // Nothing
}

void State::Enter()
{
    mFlags = Flags::None;
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
    return ((mFlags & Flags::Complete) != Flags::None);
}

void State::RequestState(IdString id)
{
    mStateRequest = id;
    mFlags |= Flags::Complete;
}
}
}

