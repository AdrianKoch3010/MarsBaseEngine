#include <MBE/Graphics/SpriteRenderComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(SpriteRenderComponent, TexturedRenderComponent)

SpriteRenderComponent::SpriteRenderComponent(Entity & parentEntity, const TextureWrapper & textureWrapper, RenderObject::RenderLayer renderLayer) :
	TexturedRenderComponent(parentEntity, textureWrapper, renderLayer),
	sprite(textureWrapper.GetTexture())
{
	assert(parentEntity.HasComponent<TransformComponent>() && "SpriteRenderComponent: The parentEntity must have a mbe::TransformComponent");
}

void SpriteRenderComponent::Draw(sf::RenderTarget & target) const
{
	sprite.setTexture(this->GetTextureWrapper().GetTexture());
	sprite.setTextureRect(this->GetTextureRect());

	target.draw(sprite, parentEntity.GetComponent<TransformComponent>().GetWorldTransform());

	// Add effects maybe get from an effects component
}

sf::FloatRect SpriteRenderComponent::GetBoundingBox() const
{
	return parentEntity.GetComponent<TransformComponent>().GetWorldTransform().transformRect(sprite.getLocalBounds());
}