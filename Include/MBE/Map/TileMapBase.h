#pragma once

#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Map/TiledTerrain.h>

namespace mbe
{
	class TileMapBase
	{
	public:
		typedef std::unique_ptr<TileMapBase> Ptr;
		typedef sf::Vector2u Position;
		typedef std::vector<mbe::Entity::HandleID> EntityIdList;

	public:
		TileMapBase() = default;
		virtual ~TileMapBase() = 0;

	public:
		virtual sf::Vector2u GetSize() const = 0;
		virtual mbe::TiledTerrain & GetTiledTerrain() const = 0;

		virtual std::vector<Position> GetReachableTiles(unsigned int x, unsigned int y) const = 0;
		virtual inline std::vector<Position> GetReachableTiles(Position position) const { return std::move(GetReachableTiles(position.x, position.y)); };

		virtual bool IsTileWalkable(unsigned int x, unsigned int y) const = 0;
		virtual bool IsTileWalkable(Position position) const { return IsTileWalkable(position.x, position.y); };

		virtual float GetTileMovementSpeed(unsigned int x, unsigned int y) const = 0;
		virtual float GetTileMovementSpeed(Position position) const { return GetTileMovementSpeed(position.x, position.y); };

		virtual const EntityIdList & GetEntities(unsigned int x, unsigned int y) const = 0;
		virtual inline const EntityIdList & GetEntities(Position position) const { return GetEntities(position.x, position.y); }

	public:
		// Converts the tile position to a float coordinate position
		sf::Vector2f MapTileToCoord(Position tilePostion) const;

		// Converts the world coordinate to a tile position
		// Throws if the coordinate is negative
		Position MapCoordToTile(sf::Vector2f coordinate) const;
	};

} // namespace mbe