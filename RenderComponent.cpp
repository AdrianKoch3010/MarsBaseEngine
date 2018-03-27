#include "RenderComponent.h"

using namespace mbe;

RenderComponent::RenderComponent(Entity & parentEntity, RenderLayer renderLayer) :
	Component(parentEntity),
	renderLayer(renderLayer),
	zOrder(0.f)
{
}
