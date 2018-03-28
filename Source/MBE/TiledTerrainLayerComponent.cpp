#include <MBE/TiledTerrainLayerComponent.h>

using namespace mbe;

TiledTerrainLayerComponent::TiledTerrainLayerComponent(Entity & parentEntity, sf::Vector2u size, sf::Vector2u tileSize) :
	Component(parentEntity),
	size(size),
	tileSize(tileSize)
{
}

size_t TiledTerrainLayerComponent::GetTile(sf::Vector2u position) const
{
	// This should never be the case
	assert(position.x * position.y < tileIndexList.size() && "TiledTerrainLayerComponent: There are less tiles registered than required by the map size");

	if (position.x >= this->size.x || position.y >= this->size.y)
		throw std::runtime_error("TiledTerrainLayerComponent: The map is smaller than the requested tile");

	return tileIndexList[position.y * size.x + position.x];
}
