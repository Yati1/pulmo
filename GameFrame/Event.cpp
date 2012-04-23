#include "Event.h"

namespace EventSystem
{
	EventPtr makeEvent( Event::TypeId type )
	{
		return EventPtr( new Event( type ) );
	}
}
