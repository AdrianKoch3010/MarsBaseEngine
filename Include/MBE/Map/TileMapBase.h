#pragma once

/// @file
/// @brief Class mbe::TileMapBase

#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Map/TiledTerrain.h>
#include <MBE/Core/Entity.h>

namespace mbe
{
	/// @brief Base class for tile maps
	/// @details
	class TileMapBase : sf::NonCopyable
	{
	public:
		typedef std::unique_ptr<TileMapBase> Ptr;
		typedef sf::Vector2i Position;
		typedef std::vector<mbe::Entity::HandleID> EntityIdList;

	public:
		TileMapBase() = default;
		~TileMapBase() = default;

	public:
		virtual sf::Vector2u GetSize() const = 0;
		virtual mbe::TiledTerrain& GetTiledTerrain() const = 0;

		virtual std::vector<Position> GetReachableTiles(int x, int y) const = 0;
		inline std::vector<Position> GetReachableTiles(Position position) const { return std::move(GetReachableTiles(position.x, position.y)); };

		virtual bool IsTileWalkable(int x, int y) const = 0;
		inline bool IsTileWalkable(Position position) const { return IsTileWalkable(position.x, position.y); };

		virtual float GetTileMovementSpeed(int x, int y) const = 0;
		inline float GetTileMovementSpeed(Position position) const { return GetTileMovementSpeed(position.x, position.y); };

		virtual const EntityIdList& GetEntities(int x, int y) const = 0;
		inline const EntityIdList& GetEntities(Position position) const { return GetEntities(position.x, position.y); }

	public:
		/// @brief Converts from the tile to the game coordinate system
		/// @details This will be the bottom left corner of the tile. The function also converts the int to a float coordinate.
		/// @param tilePosition The position in the tile map coordinate system
		/// @returns The position in the game coordinate system
		sf::Vector2f MapTileToCoord(const Position& tilePosition) const;

		/// @brief Converts from the tile to the game coordinate system
		/// @details This will be the center of the tile. The function also converts the int to a float coordinate.
		/// @param tilePosition The position in the tile map coordinate system
		/// @returns The position in the game coordinate system
		sf::Vector2f MapTileToCoordCenter(const Position& tilePosition) const;

		/// @brief Converts from the game to the time coordinate system
		/// @details The position on the tile is ignored. The function only reurns which tile the coordinate is on.
		/// @param coordinate The position in the game coordinate system
		/// @returns The tile of which the game coordinate is part of
		/// @note If the game coordinate position is exactly at the border, it is still part of the above tile
		Position MapCoordToTile(const sf::Vector2f& coordinate) const;

		/// @brief Returns the offset from the tile 
		/// @details This function calls MapCoordToTile() to find the tile the coordinate is on.
		/// Therefore, the origin of the tile is at the bottom left corner of the tile.
		/// The offset uses the standard SFML coordinate syste; i.e. The positive y-direction is down.
		/// @param coordinate The position in the game coordinate system
		/// @returns The offset from the origin of the tile the coordinate is on
		/// @see IsOffsetFromTile, GetOffsetFromTileCenter, IsOffsetFromTileCenter
		sf::Vector2f GetOffsetFromTile(const sf::Vector2f& coordinate) const;

		/// @Checks whether the coordinate is offset from the tile
		/// @details The function calls GetOffsetFromTile() and compares the return value withe a zero offset.
		/// @returns False if the coordinate is on the origin of one of the tiles, true otherwise
		/// @see GetOffsetFromTile, GetOffsetFromTileCenter, IsOffsetFromTileCenter
		bool IsOffsetFromTile(const sf::Vector2f& coordinate) const;

		/// @brief Returns the offset from the tile center
		/// @details This function calls MapCoordToTileCenter() to find the tile the coordinate is on.
		/// The offset uses the standard SFML coordinate syste; i.e. The positive y-direction is down.
		/// @param coordinate The position in the game coordinate system
		/// @returns The offset from the center of the tile the coordinate is on
		sf::Vector2f GetOffsetFromTileCenter(const sf::Vector2f& coordinate) const;

		/// @Checks whether the coordinate is offset from the tile
		/// @details The function calls GetOffsetFromTileCenter() and compares the return value withe a zero offset.
		/// @returns False if the coordinate is on the center of the tile, true otherwise
		/// @see IsOffsetFromTile, GetOffsetFromTileCenter, GetOffsetFromTile
		bool IsOffsetFromTileCenter(const sf::Vector2f& coordinate) const;
	};

} // namespace mbe