#include<MBE/Graphics/TiledTerrainLayerRenderSystem.h>

using namespace mbe;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<TextureWrapperComponent>;

TiledTerrainLayerRenderSystem::TiledTerrainLayerRenderSystem(EventManager & eventManager, const EntityManager & entityManager) :
	eventManager(eventManager),
	BaseComponentRenderSystem(entityManager)
{
	textureChangedSubscription = eventManager.Subscribe(EventManager::TCallback<TextureWrapperChangedEvent>([this](const TextureWrapperChangedEvent & event) {
		if (event.IsValueChanged("TextureWrapper"))
		{
			OnTextureChangedEvent(event.GetComponent());
		}
	}));

}

TiledTerrainLayerRenderSystem::~TiledTerrainLayerRenderSystem()
{
	eventManager.UnSubscribe<TextureWrapperChangedEvent>(textureChangedSubscription);
}

void TiledTerrainLayerRenderSystem::Update()
{
	for (const auto entityId : entityManager.GetComponentGroup<TiledTerrainLayerRenderComponent>())
	{
		assert(Entity::GetObjectFromID(entityId) != nullptr && "TiledTerrainLayerRenderSystem: The entity must exists");

		auto & entity = *Entity::GetObjectFromID(entityId);
		auto & renderComponent = entity.GetComponent<TiledTerrainLayerRenderComponent>();

		auto renderStates = renderComponent.GetRenderStates();
		
		if (entity.HasComponent<TransformComponent>())
			renderStates.transform = entity.GetComponent<TransformComponent>().GetWorldTransform();

		// Update the TiledTerraiLayerRenderComponent's render states
		renderComponent.SetRenderStates(std::move(renderStates));
	}
}

void TiledTerrainLayerRenderSystem::OnTextureChangedEvent(const TextureWrapperComponent & textureWrapperComponent)
{
	auto & entity = textureWrapperComponent.GetParentEntity();

	// Use event Subscription to update the texture
	if (!entity.HasComponent<TiledTerrainLayerRenderComponent>())
		return;

	auto & renderComponent = entity.GetComponent<TiledTerrainLayerRenderComponent>();

	// Update the texture
	auto renderStates = renderComponent.GetRenderStates();

	renderStates.texture = &entity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();

	// The move must be in the end
	renderComponent.SetRenderStates(std::move(renderStates));
}