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

void TileMapComponent::Rotate()
{
	auto oldSize = this->GetSize();
	MovementSpeedShape newShape;

	// Push back enough empty row vectors
	for (size_t i = 0; i < oldSize.x; i++)
		newShape.push_back(std::vector<float>());

	int newRowCounter = 0;
	for (int i = oldSize.x - 1; i >= 0; i--, newRowCounter++)
	{
		for (size_t j = 0; j < oldSize.y; j++)
			newShape.at(newRowCounter).push_back(GetMovementSpeed(i, j));
	}

	movementSpeedShape = newShape;
}

