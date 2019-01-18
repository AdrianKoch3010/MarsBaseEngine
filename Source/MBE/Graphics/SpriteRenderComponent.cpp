#include <MBE/Graphics/SpriteRenderComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(SpriteRenderComponent, RenderComponent)

SpriteRenderComponent::SpriteRenderComponent(Entity & parentEntity) :
	RenderComponent(parentEntity),
	transform(sf::Transform::Identity)
{
}

void SpriteRenderComponent::Draw(sf::RenderTarget & target) const
{
	// Add effects maybe get from an effects component
	
	target.draw(sprite, transform);
}

sf::FloatRect SpriteRenderComponent::GetLocalBounds() const
{
	return sprite.getLocalBounds();
}
