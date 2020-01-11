#include "..\..\..\Include\MBE\Map\TileMapComponent.h"
#include <MBE/Map/TileMapComponent.h>

using namespace mbe;

TileMapComponent::TileMapComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity),
	position({ 0u, 0u })
{

}

float TileMapComponent::GetMovementSpeed(unsigned int x, unsigned int y) const
{
	if (y >= movementSpeedShape.size() || x >= movementSpeedShape[y].size())
		throw std::out_of_range("TileMapComponent: The requested position does not fit within the shape");

	return movementSpeedShape[y][x];
}

sf::Vector2u mbe::TileMapComponent::GetSize() const
{
	sf::Vector2u size;
	size.y = movementSpeedShape.size();

	// If the height is not 0
	if (size.y != 0)
		size.x = movementSpeedShape.front().size();

	return size;
}

void TileMapComponent::SetMovementSpeed(unsigned int x, unsigned int y, float speed)
{
	if (y >= movementSpeedShape.size() || x >= movementSpeedShape[y].size())
		throw std::out_of_range("TileMapComponent: The requested position does not fit within the shape");

	movementSpeedShape[y][x] = speed;
}

