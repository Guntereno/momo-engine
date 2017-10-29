#ifndef MOMO_INPUT_TOUCHEVENT_INCLUDED
#define MOMO_INPUT_TOUCHEVENT_INCLUDED

#include "Momo.h"

#include "Point.h"


namespace Momo
{
	namespace Input
	{

		struct Event
		{
			typedef u8 Id;

			enum Type
			{
				kNone,
				kDown,
				kMove,
				kUp
			};

			Type type;
			Id id;
			const Point pos;
			const Point delta;

			static const char* ToString(Event::Type type)
			{
				switch (type)
				{
				case kNone: return "None";
				case kDown: return "Down";
				case kMove: return "Move";
				case kUp: return "Up";

				default:
					BREAK_MSG("Invalid type %d!", type);
					return "Invalid";
				}
			}
		};

	}
}

#endif //MOMO_INPUT_TOUCHEVENT_INCLUDED
