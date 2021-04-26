#include <MBE/Graphics/SpriteRenderSystem.h>

using namespace mbe;
using mbe::event::EntityCreatedEvent;

SpriteRenderSystem::SpriteRenderSystem(EventManager& eventManager, const EntityManager& entityManager) :
	BaseComponentRenderSystem(entityManager),
	eventManager(eventManager)
{
	entityCreatedSubscription = eventManager.Subscribe(EventManager::TCallback<EntityCreatedEvent>([this](const EntityCreatedEvent& event)
		{
			OnEntityCreatedEvent(*event.GetEntityID());
		}));
}

SpriteRenderSystem::~SpriteRenderSystem()
{
	eventManager.UnSubscribe<EntityCreatedEvent>(entityCreatedSubscription);
}

void SpriteRenderSystem::Update()
{
	auto& spriteEntityIdList = entityManager.GetComponentGroup<SpriteRenderComponent>();

	for (auto& entityId : spriteEntityIdList)
	{
		auto& renderComponent = entityId->GetComponent<SpriteRenderComponent>();

		// Set the sprite texture and texture rect if the entity has a mbe::TextureWrapperComponent
		// This effects the size of the sprite as well hence it should be updated outside of the draw method
		if (entityId->HasComponent<TextureWrapperComponent>())
		{
			renderComponent.SetTexture(entityId->GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture());
			renderComponent.SetTextureRect(entityId->GetComponent<TextureWrapperComponent>().GetTextureRect());
		}

		// Set the sprite position if the entity has a mbe::TransformComponent
		auto transform = sf::Transform::Identity;
		if (entityId->HasComponent<TransformComponent>())
			transform = entityId->GetComponent<TransformComponent>().GetWorldTransform();
		renderComponent.SetTransform(transform);
	}
}

void SpriteRenderSystem::OnEntityCreatedEvent(Entity& entity)
{
	if (!entity.HasComponent<mbe::SpriteRenderComponent>())
		return;

	auto& renderComponent = entity.GetComponent<SpriteRenderComponent>();

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
