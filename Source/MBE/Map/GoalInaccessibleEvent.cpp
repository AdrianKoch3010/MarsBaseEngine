#include <MBE/Map/GoalInaccessibleEvent.h>

using namespace mbe;
using namespace mbe::event;

GoalInaccessibleEvent::GoalInaccessibleEvent() :
	entityId(Entity::GetNullID())
{
}

GoalInaccessibleEvent::GoalInaccessibleEvent(Entity::ID entityId) :
	entityId(entityId)
{
}

std::ostream& mbe::event::operator<<(std::ostream& stream, const GoalInaccessibleEvent& event)
{
	stream << "GoalInaccessibleEvent:\tEntityId: ";
	stream << event.GetEntityID();
	return stream;
}
