#ifdef ANDROID

#include "Input\TouchHandler.h"

#include <cstring>


namespace Momo
{
namespace Input
{
TouchHandler::TouchHandler() :
    mNumTouches(0)
{
    memset(mTouches, 0, sizeof(mTouches));
}
}
}

#endif //ANDROID