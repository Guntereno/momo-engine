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
	static const IdString kNull;

	// Assumes null terminated string
	IdString(const char* pString)
	{
		mHash = Crc32(pString, strlen(pString));
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

