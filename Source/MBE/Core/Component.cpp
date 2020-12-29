#include <MBE/Core/Component.h>

using namespace mbe;

Component::Component(EventManager& eventManager, Entity& parentEntity) :
	eventManager(eventManager),
	parentEntity(parentEntity)
{
}