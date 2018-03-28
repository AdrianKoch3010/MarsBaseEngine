#include <MBE/Core/BaseEvent.h>

using mbe::detail::BaseEvent;

BaseEvent::TypeID mbe::detail::BaseEvent::GetNextID()
{
	static TypeID id = 0;
	return id++;
}
