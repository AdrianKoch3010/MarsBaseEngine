#pragma once

#include <memory>
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

	public:
		TileMapBase() = default;
		virtual ~TileMapBase() = 0;

	public:
		virtual sf::Vector2u GetSize() const = 0;
		virtual mbe::TiledTerrain & GetTileMap() const = 0;

		virtual std::vector<Position> GetReachableTiles(unsigned int x, unsigned int y) const = 0;
		virtual inline std::vector<Position> GetReachableTiles(Position position) const = 0;

		virtual bool IsTileWalkable(unsigned int x, unsigned int y) const = 0;
		virtual bool IsTileWalkable(Position position) const /*{ return IsTileWalkable(position.x, position.y); }*/ = 0;

		virtual float GetTileMovementSpeed(unsigned int x, unsigned int y) const = 0;
		virtual float GetTileMovementSpeed(Position position) const /*{ return GetTileMovementSpeed(position.x, position.y); }*/ = 0;
	};
} // namespace mbe