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

	// automatically floored
	position.x = coordinate.x / static_cast<float>(this->GetTiledTerrain().GetTileSize().x);
	position.y = coordinate.y / static_cast<float>(this->GetTiledTerrain().GetTileSize().y);

	return position;
}
