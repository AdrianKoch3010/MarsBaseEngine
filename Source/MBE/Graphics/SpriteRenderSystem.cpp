#include <MBE/Graphics/SpriteRenderSystem.h>

using namespace mbe;

SpriteRenderSystem::SpriteRenderSystem(const EntityManager & entityManager) :
	BaseComponentRenderSystem(entityManager)
{

}

void SpriteRenderSystem::Update()
{
	const auto & spriteEntityIdList = entityManager.GetComponentGroup<SpriteRenderComponent>();

	for (const auto entityId : spriteEntityIdList)
	{
		assert(Entity::GetObjectFromID(entityId) != nullptr && "SpriteRenderSystem: The entity must exists");

		auto & entity = *Entity::GetObjectFromID(entityId);
		auto & renderComponent = entity.GetComponent<SpriteRenderComponent>();

		// Set the sprite texture and texture rect if the entity has a mbe::TextureWrapperComponent
		// This effects the size of the sprite as well hence it should be updated outside of the draw method
		if (entity.HasComponent<TextureWrapperComponent>())
		{
			renderComponent.SetTexture(entity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture());
			renderComponent.SetTextureRect(entity.GetComponent<TextureWrapperComponent>().GetTextureRect());
		}

		// Set the sprite position if the entity has a mbe::TransformComponent
		auto transform = sf::Transform::Identity;
		if (entity.HasComponent<TransformComponent>())
			transform = entity.GetComponent<TransformComponent>().GetWorldTransform();
		renderComponent.SetTransform(transform);
	}
}
