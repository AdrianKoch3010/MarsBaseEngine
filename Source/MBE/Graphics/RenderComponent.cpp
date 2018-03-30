#include <MBE/Graphics/RenderComponent.h>

using namespace mbe;

RenderComponent::RenderComponent(Entity & parentEntity, RenderObject::RenderLayer renderLayer) :
	Component(parentEntity),
	RenderObject(renderLayer)
{
}
