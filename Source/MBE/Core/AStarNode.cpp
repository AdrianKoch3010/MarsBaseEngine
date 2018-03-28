#include <MBE/Core/AStarNode.h>

using mbe::detail::AStarNode;

AStarNode::AStarNode(Position position) :
	position(position),
	g(std::numeric_limits<float>::max()),
	h(0),
	//previousNode(nullptr),
	open(false),
	closed(false)
{
}

float AStarNode::DistanceTo(Position endPosition)
{
	// Manhatten distance from this node to the passed in end node
	//return std::abs(std::get<0>(this->position) - std::get<0>(endPosition)) + std::abs(std::get<1>(this->position) - std::get<1>(endPosition));

	// Pythagoras distance from this node to the passed in end node
	return std::sqrt(std::pow((float)this->position.x - (float)endPosition.x, 2.f) + std::pow((float)this->position.y - (float)endPosition.y, 2.f));
}