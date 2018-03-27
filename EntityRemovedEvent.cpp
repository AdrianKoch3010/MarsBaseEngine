#include "EntityRemovedEvent.h"

using mbe::Entity;
using mbe::event::EntityRemovedEvent;

EntityRemovedEvent::EntityRemovedEvent() :
	entityId(Entity::GetNullID())
{
}

EntityRemovedEvent::EntityRemovedEvent(Entity::HandleID entityId) :
	entityId(entityId)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const EntityRemovedEvent & event)
{
	stream << "EntityRemovedEvent:\tEntityId: ";
	stream << event.GetEntityID();
	return stream;
}
