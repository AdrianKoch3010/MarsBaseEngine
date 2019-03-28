#include <MBE/Map/TileMapBase.h>

using namespace mbe;

sf::Vector2f TileMapBase::MapTileToCoord(Position tilePostion) const
{
	sf::Vector2f coordinate;

	coordinate.x = tilePostion.x * (float)this->GetTiledTerrain().GetTileSize().x;
	coordinate.y = tilePostion.y * (float)this->GetTiledTerrain().GetTileSize().y + (float)this->GetTiledTerrain().GetTileSize().y;

	return coordinate;
}

TileMapBase::Position TileMapBase::MapCoordToTile(sf::Vector2f coordinate) const
{
	Position position;

	// automatically floored (stored in int)
	position.x = std::floor(coordinate.x / this->GetTiledTerrain().GetTileSize().x);
	position.y = std::floor(coordinate.y / this->GetTiledTerrain().GetTileSize().y);

	// If its exactly at the border its still part of the above tile
	if ((coordinate.y / (float)this->GetTiledTerrain().GetTileSize().y) - static_cast<int>(coordinate.y / (float)this->GetTiledTerrain().GetTileSize().y) == 0)
		position.y--;

	return position;
}

sf::Vector2f TileMapBase::GetOffsetFromTile(sf::Vector2f coordinate) const
{
	Position tilePosition = MapCoordToTile(coordinate);
	sf::Vector2f position;
	position.x = (float)tilePosition.x * (float)this->GetTiledTerrain().GetTileSize().x;
	position.y = (float)tilePosition.y * (float)this->GetTiledTerrain().GetTileSize().y + (float)this->GetTiledTerrain().GetTileSize().y;

	return coordinate - position;
}

bool TileMapBase::IsOffsetFromTile(sf::Vector2f coordinate) const
{
	return GetOffsetFromTile(coordinate) != sf::Vector2f(0.f, 0.f);
}
