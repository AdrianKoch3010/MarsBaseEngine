#include "EntityCreatedEvent.h"

using mbe::Entity;
using mbe::event::EntityCreatedEvent;

EntityCreatedEvent::EntityCreatedEvent() :
	entityId(Entity::GetNullID())
{
}

EntityCreatedEvent::EntityCreatedEvent(Entity::HandleID entityId) :
	entityId(entityId)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const EntityCreatedEvent & event)
{
	stream << "EntityCreatedEvent:\tEntityId: ";
	stream << event.GetEntityID();
	return stream;
}
