#include <MBE/Core/ComponentsChangedEvent.h>
#include <MBE/Core/Entity.h>

using mbe::Entity;
using mbe::event::ComponentsChangedEvent;

ComponentsChangedEvent::ComponentsChangedEvent(Entity & entity) :
	entity(entity)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const ComponentsChangedEvent & event)
{
	stream << "ComponentsChangedEvent:\tEntityId: ";
	stream << event.GetEntity().GetHandleID();
	return stream;
}
