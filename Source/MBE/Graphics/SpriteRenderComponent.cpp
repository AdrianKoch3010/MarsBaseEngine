#include <MBE/Graphics/SpriteRenderComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(SpriteRenderComponent, RenderComponent)

SpriteRenderComponent::SpriteRenderComponent(Entity & parentEntity) :
	RenderComponent(parentEntity),
	transform(sf::Transform::Identity)
{
}

void SpriteRenderComponent::Update(sf::Time deltaTime)
{
	// Set the sprite texture and texture rect if the entity has a mbe::TextureWrapperComponent
	// This effects the size of the sprite as well hence it should be updated outside of the draw method
	/*if (parentEntity.HasComponent<TextureWrapperComponent>())
	{
		sprite.setTexture(parentEntity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture());
		sprite.setTextureRect(parentEntity.GetComponent<TextureWrapperComponent>().GetTextureRect());
	}*/
}

void SpriteRenderComponent::Draw(sf::RenderTarget & target) const
{
	// Set the sprite position if the entity has a mbe::TransformComponent
	/*auto transform = sf::Transform::Identity;
	if (parentEntity.HasComponent<TransformComponent>())
		transform = parentEntity.GetComponent<TransformComponent>().GetWorldTransform();*/

	// Add effects maybe get from an effects component
	
	target.draw(sprite, transform);
}

sf::FloatRect SpriteRenderComponent::GetLocalBounds() const
{
	return sprite.getLocalBounds();
}
