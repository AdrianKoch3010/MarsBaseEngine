#include <MBE/Map/TileMapBase.h>

using namespace mbe;

sf::Vector2f TileMapBase::MapTileToCoord(const Position& tilePosition) const
{
	sf::Vector2f coordinate;

	coordinate.x = (float)tilePosition.x * (float)GetTiledTerrain().GetTileSize().x;
	coordinate.y = (float)tilePosition.y * (float)GetTiledTerrain().GetTileSize().y + (float)GetTiledTerrain().GetTileSize().y;

	return coordinate;
}

sf::Vector2f TileMapBase::MapTileToCoordCenter(const Position& tilePosition) const
{
	sf::Vector2f coordinate;

	coordinate.x = (float)tilePosition.x * (float)GetTiledTerrain().GetTileSize().x + ((float)GetTiledTerrain().GetTileSize().x / 2.f);
	coordinate.y = (float)tilePosition.y * (float)GetTiledTerrain().GetTileSize().y + ((float)GetTiledTerrain().GetTileSize().y / 2.f);

	return coordinate;
}

TileMapBase::Position TileMapBase::MapCoordToTile(const sf::Vector2f& coordinate) const
{
	Position position;
	auto tileSize = GetTiledTerrain().GetTileSize();

	// automatically floored (stored in int)
	position.x = std::floor(coordinate.x / tileSize.x);
	position.y = std::floor(coordinate.y / tileSize.y);

	// If its exactly at the border its still part of the above tile
	if ((coordinate.y / (float)tileSize.y) - static_cast<int>(coordinate.y / (float)tileSize.y) == 0)
		position.y--;

	return position;
}

sf::Vector2f TileMapBase::GetOffsetFromTile(const sf::Vector2f& coordinate) const
{
	Position tilePosition = MapCoordToTile(coordinate);

	return coordinate - MapTileToCoord(tilePosition);
}

bool TileMapBase::IsOffsetFromTile(const sf::Vector2f& coordinate) const
{
	return GetOffsetFromTile(coordinate) != sf::Vector2f(0.f, 0.f);
}

sf::Vector2f TileMapBase::GetOffsetFromTileCenter(const sf::Vector2f& coordinate) const
{
	Position tilePosition = MapCoordToTile(coordinate);

	return coordinate - MapTileToCoordCenter(tilePosition);
}

bool TileMapBase::IsOffsetFromTileCenter(const sf::Vector2f& coordinate) const
{
	return GetOffsetFromTileCenter(coordinate) != sf::Vector2f(0.f, 0.f);
}
