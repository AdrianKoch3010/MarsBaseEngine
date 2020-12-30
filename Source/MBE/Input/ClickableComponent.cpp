#include <MBE/Input/ClickableComponent.h>

using namespace mbe;

ClickableComponent::ClickableComponent(EventManager & eventManager, Entity & parentEntity)
	: Component(eventManager, parentEntity),
	absorbeClick(true),
	bubbleUp(true)
{
}
