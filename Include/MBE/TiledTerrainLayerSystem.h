#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerSystem

#include <MBE/Core/EventManager.h>
#include <MBE/Core/Entity.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/TiledTerrainLayerComponent.h>
#include <MBE/Graphics/TiledTerrainLayerRenderComponent.h>

namespace mbe
{

	class TiledTerrainLayerSystem
	{
	public:
		TiledTerrainLayerSystem(EventManager & eventManager);
		~TiledTerrainLayerSystem() = default;

	public:
		void OnEntityCreated(Entity & entity);
		
		/// @brief Sets a single tile's index
		/// @param tileIndex The index of the tile on the tile set texture. The tile index goes left to right, top to bottom
		/// so basically row by row.
		/// @param position The position in tiles on the tile map at which the tile shall be set
		void SetTile(Entity & entity, size_t tileIndex, sf::Vector2u position);

	private:
		EventManager & eventManager;
	};

} // namespace mbe