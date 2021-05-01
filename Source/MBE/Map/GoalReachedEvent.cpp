#include <MBE/Map/GoalReachedEvent.h>

using namespace mbe;
using namespace mbe::event;

GoalReachedEvent::GoalReachedEvent() :
	entityId(Entity::GetNullID())
{
}

GoalReachedEvent::GoalReachedEvent(Entity::ID entityId) :
	entityId(entityId)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const GoalReachedEvent & event)
{
	stream << "GoalReachedEvent:\tEntityId: ";
	stream << event.GetEntityID();
	return stream;
}
