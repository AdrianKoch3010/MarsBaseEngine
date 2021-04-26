#include<MBE/Graphics/TiledTerrainLayerRenderSystem.h>

using namespace mbe;
using mbe::event::EntityCreatedEvent;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<TextureWrapperComponent>;

TiledTerrainLayerRenderSystem::TiledTerrainLayerRenderSystem(EventManager& eventManager, const EntityManager& entityManager) :
	eventManager(eventManager),
	BaseComponentRenderSystem(entityManager)
{
	/*textureChangedSubscription = eventManager.Subscribe(EventManager::TCallback<TextureWrapperChangedEvent>([this](const TextureWrapperChangedEvent & event) {
		if (event.IsValueChanged("TextureWrapper"))
		{
			OnTextureChangedEvent(event.GetComponent());
		}
	}));*/

	entityCreatedSubscription = eventManager.Subscribe(EventManager::TCallback<EntityCreatedEvent>([this](const EntityCreatedEvent& event)
		{
			OnEntityCreatedEvent(*event.GetEntityID());
		}));

}

TiledTerrainLayerRenderSystem::~TiledTerrainLayerRenderSystem()
{
	eventManager.UnSubscribe<TextureWrapperChangedEvent>(entityCreatedSubscription);
}

void TiledTerrainLayerRenderSystem::Update()
{
	for (auto& entityId : entityManager.GetComponentGroup<TiledRenderComponent>())
	{
		auto& renderComponent = entityId->GetComponent<TiledRenderComponent>();

		auto renderStates = renderComponent.GetRenderStates();

		// Set the transform if the entity has a transform component
		if (entityId->HasComponent<TransformComponent>())
			renderStates.transform = entityId->GetComponent<TransformComponent>().GetWorldTransform();

		// Update the TiledTerraiLayerRenderComponent's render states
		renderComponent.SetRenderStates(std::move(renderStates));
	}
}

//void TiledTerrainLayerRenderSystem::OnTextureChangedEvent(const TextureWrapperComponent & textureWrapperComponent)
//{
//	auto & entity = textureWrapperComponent.GetParentEntity();
//
//	// Use event Subscription to update the texture
//	if (!entity.HasComponent<TiledRenderComponent>())
//		return;
//
//	auto & renderComponent = entity.GetComponent<TiledRenderComponent>();
//
//	// Update the texture
//	auto renderStates = renderComponent.GetRenderStates();
//
//	renderStates.texture = &entity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();
//
//	// The move must be in the end
//	renderComponent.SetRenderStates(std::move(renderStates));
//}

void TiledTerrainLayerRenderSystem::OnEntityCreatedEvent(Entity& entity)
{
	if (!entity.HasComponent<TextureWrapperComponent>() || !entity.HasComponent<TiledRenderComponent>())
		return;

	auto& renderComponent = entity.GetComponent<TiledRenderComponent>();

	// Update the texture
	auto renderStates = renderComponent.GetRenderStates();

	renderStates.texture = &entity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();

	// The move must be in the end
	renderComponent.SetRenderStates(std::move(renderStates));
}
