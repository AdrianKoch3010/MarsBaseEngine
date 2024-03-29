#include <MBE/Core/EntityCreatedEvent.h>

using mbe::Entity;
using mbe::event::EntityCreatedEvent;

EntityCreatedEvent::EntityCreatedEvent() :
	entityId(Entity::GetNullID())
{
}

EntityCreatedEvent::EntityCreatedEvent(Entity::ID entityId) :
	entityId(entityId)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const EntityCreatedEvent & event)
{
	stream << "EntityCreatedEvent:\tEntityId: ";
	stream << event.GetEntityID();
	return stream;
}
