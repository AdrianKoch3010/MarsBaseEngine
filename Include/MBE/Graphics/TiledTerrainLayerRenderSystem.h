#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerRenderSystem

#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <MBE/Graphics/BaseComponentRenderSystem.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/ComponentValueChangedEvent.h>

#include <MBE/Graphics/TiledTerrainLayerRenderComponent.h>
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
		// The tile map does the same thing
		// Maybe make this the responsibility of the tile map
		void OnTextureChangedEvent(const TextureWrapperComponent & component);

	private:
		EventManager & eventManager;
		EventManager::SubscriptionID textureChangedSubscription;
	};

} // namespace mbe