#include <MBE/Map/TileMapBase.h>

using namespace mbe;

sf::Vector2f TileMapBase::MapTileToCoord(Position tilePostion) const
{
	sf::Vector2f coordinate;

	coordinate.x = tilePostion.x * this->GetTiledTerrain().GetTileSize().x;
	coordinate.y = tilePostion.y * this->GetTiledTerrain().GetTileSize().y;

	return coordinate;
}

TileMapBase::Position TileMapBase::MapCoordToTile(sf::Vector2f coordinate) const
{
	if (coordinate.x < 0 || coordinate.y < 0)
		throw std::runtime_error("TileMapBase: The coordinate must be positive");

	Position position;

	// automatically floored (stored in unsigend int)
	position.x = coordinate.x / static_cast<float>(this->GetTiledTerrain().GetTileSize().x);
	position.y = coordinate.y / static_cast<float>(this->GetTiledTerrain().GetTileSize().y);

	return position;
}

sf::Vector2f TileMapBase::GetOffsetFromTile(sf::Vector2f coordinate) const
{
	Position tilePosition = MapCoordToTile(coordinate);
	sf::Vector2f position;
	position.x = static_cast<float>(tilePosition.x) * static_cast<float>(this->GetTiledTerrain().GetTileSize().x);
	position.y = static_cast<float>(tilePosition.y) * static_cast<float>(this->GetTiledTerrain().GetTileSize().y);

	return coordinate - position;
}

bool TileMapBase::IsOffsetFromTile(sf::Vector2f coordinate) const
{
	return GetOffsetFromTile(coordinate) == sf::Vector2f(0.f, 0.f);
}
