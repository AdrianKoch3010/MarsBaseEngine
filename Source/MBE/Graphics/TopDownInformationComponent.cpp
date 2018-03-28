#include <MBE/Graphics/TopDownInformationComponent.h>

using namespace mbe;

TopDownInformationComponent::TopDownInformationComponent(Entity & parentEntity, float logicalBottomOffset) :
	Component(parentEntity),
	logicalBottomOffset(logicalBottomOffset)
{
}

