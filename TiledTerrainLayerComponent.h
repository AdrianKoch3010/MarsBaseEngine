#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerComponent

#include <iostream>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

#include "Entity.h"
#include "Component.h"

namespace mbe
{

	class TiledTerrainLayerComponent : public Component
	{
	public:
		/// @brief A constant defining an empty tile. This is a transparrent tile that has no texture associated with it.
		static const int emptyTile = -1;

	public:
		TiledTerrainLayerComponent(Entity & parentEntity, sf::Vector2u size, sf::Vector2u tileSize);
		~TiledTerrainLayerComponent() = default;

	public:
		inline void Update(sf::Time frameTime) override {}

		/// std::move
		inline void SetTileIndexList(std::vector<size_t> tileIndexList) { this->tileIndexList = tileIndexList; }

		// Throws
		size_t GetTile(sf::Vector2u position) const;

		inline const sf::Vector2u & GetSize() const { return size; }
		inline const sf::Vector2u & GetTileSize() const { return tileSize; }
		inline const std::vector<size_t> & GetTileIndexList() const { return tileIndexList; }

	private:
		const sf::Vector2u size;
		const sf::Vector2u tileSize;

		std::vector<size_t> tileIndexList;
	};

} // namespace mbe