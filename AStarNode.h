#pragma once

/// @file
/// @brief Class mbe::detail::AStarNode

#include <memory>

#include <SFML/System/Vector2.hpp>

namespace mbe
{
	namespace detail
	{

		class AStarNode
		{
		public:
			typedef std::shared_ptr<AStarNode> Ptr;
			typedef std::weak_ptr<AStarNode> WPtr;

			/// Change this back to being a local class --> then change Position def to typename TMap::Position Position
			typedef sf::Vector2u Position;

		public:
			/// @brief Constructor
			/// @param position The position of the node that is equally its identifier
			AStarNode(Position position);

			/// @brief Default destructor
			~AStarNode() = default;

			/// @brief Returns the combined heuristic f
			inline float GetF() const { return g + h; }

			/// @brief Calculates the distance to a position
			/// @param endPosition The position the distance is calculated to (usually the goal position of the a star algorithm)
			/// @details Both, manhatten and pythagoras distance can be used. They gain slightly different results when used for the heuristic.
			/// @returns The distance to the passed in (end)Position
			float DistanceTo(Position endPosition);

			const Position position;
			float g;
			float h;
			AStarNode::WPtr previousNode;

			// Avoids having to search through the open and close list in order to check whether a creatain node is in either
			/// @brief Flag indicating whether this node is in the open list
			bool open;

			/// @brief Flag indicating whether this node is in the closed list
			bool closed;
		};

	} // namespace detail
} // namespace mbe