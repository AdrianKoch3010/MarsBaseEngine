#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerRenderSystem

#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <MBE/Graphics/BaseComponentRenderSystem.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/ComponentValueChangedEvent.h>

#include <MBE/Graphics/TiledRenderComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/TransformComponent.h>


namespace mbe
{

	class TiledTerrainLayerRenderSystem : public BaseComponentRenderSystem
	{
	public:
		std::shared_ptr<TiledTerrainLayerRenderSystem> Ptr;
		std::weak_ptr<TiledTerrainLayerRenderSystem> WPtr;
		std::unique_ptr<TiledTerrainLayerRenderSystem> UPtr;

	public:
		TiledTerrainLayerRenderSystem(EventManager & eventManager, const EntityManager & entityManager);

		// Unsubscribe from events
		~TiledTerrainLayerRenderSystem();

	public:
		void Update() override;

	private:
		// This function is subscribed in the tiled terrain
		// The tile map does the same thing
		// Maybe make this the responsibility of the tile map
		//void OnTextureChangedEvent(const TextureWrapperComponent & component);

		void OnEntityCreatedEvent(Entity & entity);

	private:
		EventManager & eventManager;
		EventManager::SubscriptionID entityCreatedSubscription;
	};

} // namespace mbe