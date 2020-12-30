#include <MBE/Graphics/TopDownInformationComponent.h>

using namespace mbe;

TopDownInformationComponent::TopDownInformationComponent(EventManager& eventManager, Entity& parentEntity, float logicalBottomOffset) :
	Component(eventManager, parentEntity),
	logicalBottomOffset(logicalBottomOffset)
{
}

