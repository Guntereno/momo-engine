#ifndef MOMO_IDSTRING_INCLUDED
#define MOMO_IDSTRING_INCLUDED

#include "Momo.h"

#include "Util/Crc32.h"

#include <cstring>


namespace Momo
{

class IdString
{
public:
    // Assumes null terminated string
    IdString(const char* pString)
    {
        mHash = Crc32(pString, strlen(pString));
    }

    IdString()
    {
        mHash = 0;
    }

    bool operator==(const IdString& rhs)
    {
        return (this->mHash == rhs.mHash);
    }

    bool operator!=(const IdString& rhs)
    {
        return (this->mHash != rhs.mHash);
    }

private:
    u32 mHash;
};

}

#endif // MOMO_IDSTRING_INCLUDED

