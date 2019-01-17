#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <memory>
#include <cassert>

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/AStarNode.h>

/// Change assert(speed >= 1.f) to assert(speed >= defaultSpeed) where default speed may be declared in the constants class or the gird
/// Somehow make sure that the tile movement speed it always greater that 1 (the heuristic movement speed)

/// Add a template parameter for the heuristic

/// Put the GetNeighbours function into the map. This will allow for more complex neighbour identification e.g. based on inside outside.....
/// Maybe allow for using a std::function that returns a list of neighbours as an option - or a functor similar to how the std::priority_queue does it

namespace mbe
{
	enum class Heuristic : unsigned short int
	{
		PythagorasDistance,
		ManhattenDistance,
		HeuristicCount
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Class that can be used to perform a star pathfinding on a tile map
	/// @tparam TTileMap The type of tile map the pathfinding is performed on.
	/// It must provide certain functions with the following signatures:
	/// @n - Position GetSize() const
	/// @n - float GetTileMovementSpeed(unsigned int x, unsigned int y)
	/// @n - std::vector<Position> GetReachableTiles(unsigned int x, unsigned int y)
	/// @n - typedef Position with an x and a y member.
	//// @details STUFF ON MULTITHREADING
	template <class TTileMap/*, Heuristic heuristic = Heuristic::PythagorasDistance*/>
	class AStarPathfinder
	{
	public:
		typedef typename TTileMap::Position Position;

	private:
		

	private:
		struct CompareNodes
		{
			bool operator () (const detail::AStarNode::Ptr & lhs, const detail::AStarNode::Ptr & rhs) const
			{
				return lhs->GetF() > rhs->GetF();
			}
		};

	public:
		/// @brief Constructor
		/// @param tileMap A reference to the tileMap on which the pathfinding is performed
		AStarPathfinder(const TTileMap & tileMap);

		/// @brief Destructor
		~AStarPathfinder();

		/// @brief Finds the best path between two points on the tileMap
		/// @param startPos The start position
		/// @param endPos The end position
		/// @returns The path as a list of positions. If no path could be found, an empty list is returned
		std::vector<Position> FindPath(Position startPos, Position endPos);

	private:
		void ExpandNode(detail::AStarNode::Ptr currentNode, Position endPosition);

		// Takes care that only nodes are created that are examined by the algorithm
		// The actual logic of what is a neighbour on the graph lies withing the TTileMap::GetReachableTiles() method
		std::vector<detail::AStarNode::Ptr> GetNeighbours(Position nodePosition);

		inline bool DoesNodeExist(Position position) const;
		inline void SetNode(detail::AStarNode::Ptr node);
		inline detail::AStarNode::Ptr GetNode(Position position) const;

		// Clear the open and closed lists and node map
		void Reset();

	private:
		// The open and closed lists are used to keep items (std::shared_ptrs) alive
		std::priority_queue<detail::AStarNode::Ptr, std::vector<detail::AStarNode::Ptr>, CompareNodes> openList;
		std::vector<detail::AStarNode::Ptr> closedList;

		// Keeps track of the nodes that have already been created
		detail::AStarNode::Ptr * nodeMap;
		const unsigned int nodeMapLength;

		const TTileMap & map;
	};


#pragma region Template Implementation

	template <class TTileMap>
	AStarPathfinder<TTileMap>::AStarPathfinder(const TTileMap & tileMap) :
		nodeMapLength(tileMap.GetSize().x * tileMap.GetSize().y),
		map(tileMap)
	{
		//nodeMap = std::make_unique<NodeMap[]>(map.GetSize().x * map.GetSize().y);
		nodeMap = new detail::AStarNode::Ptr[nodeMapLength];
	}

	template<class TTileMap>
	inline AStarPathfinder<TTileMap>::~AStarPathfinder()
	{
		delete[] nodeMap;
	}

	template <class TTileMap>
	std::vector<detail::AStarNode::Ptr> AStarPathfinder<TTileMap>::GetNeighbours(Position nodePosition)
	{
		// Check whether the tile is in bound
		assert(nodePosition.x >= 0 && nodePosition.x < map.GetSize().x && nodePosition.y >= 0 && nodePosition.y< map.GetSize().y && "AStartPathfinder: The tile lyes outside the map");

		std::vector<Position> neighbourPositions = std::move(map.GetReachableTiles(nodePosition));
		std::vector<detail::AStarNode::Ptr> neighbours;

		for (const auto & neighbourPosition : neighbourPositions)
		{
			detail::AStarNode::Ptr nodePtr;
			// If this node has not been created yet
			if (DoesNodeExist(neighbourPosition) == false)
			{
				// Create it
				nodePtr = std::make_shared<detail::AStarNode>(neighbourPosition);
				SetNode(nodePtr);
			}
			else
			{
				// Return the existing node
				nodePtr = GetNode(neighbourPosition);
			}

			neighbours.emplace_back(nodePtr);
		}

		return neighbours;
	}

	template <class TTileMap>
	std::vector<typename AStarPathfinder<TTileMap>::Position> AStarPathfinder<TTileMap>::FindPath(Position startPos, Position endPos)
	{
		Reset();

		// Create the start node
		auto startNode = std::make_shared<detail::AStarNode>(startPos);
		startNode->g = 0;
		startNode->h = startNode->DistanceTo(endPos);


		startNode->open = true;
		openList.push(startNode);


		while (!openList.empty())
		{
			// Get the most promising node from the openList (The current node is kept alive)
			auto currentNode = openList.top();
			currentNode->open = false;
			// Current node can be popped since it is the last item in the vector through the sort
			openList.pop();

			// Add the currentNode to the closed list in order to prevent cycles
			currentNode->closed = true;
			closedList.push_back(currentNode);

			// End condition
			if (currentNode->position == endPos)
			{
				// FOUND! Return the path
				std::vector<Position> path;

				while (currentNode != nullptr)
				{
					path.push_back(currentNode->position);
					currentNode = currentNode->previousNode.lock();
				}

				// Reverse the path
				std::reverse(path.begin(), path.end());
				return path;
			}

			this->ExpandNode(currentNode, endPos);
		}

		// No Path has been found, return an emtpy list
		return std::vector<Position>();
	}

	template <class TTileMap>
	void AStarPathfinder<TTileMap>::Reset()
	{
		while (!openList.empty())
			openList.pop();

		closedList.clear();

		//std::fill(std::begin(nodeMap), std::end(nodeMap), nullptr);

		for (size_t i = 0; i < nodeMapLength; i++)
			nodeMap[i].reset();
	}

	template <class TTileMap>
	void AStarPathfinder<TTileMap>::ExpandNode(detail::AStarNode::Ptr currentNode, Position endPosition)
	{
		for (const auto & succeedingNode : this->GetNeighbours(currentNode->position))
		{
			// If the succeeding node is already on the closed list, continue
			if (succeedingNode->closed)
				continue;

			// The movement cost to that node --> maybe add something more fancy later on
			auto edgeValue = map.GetTileMovementSpeed(succeedingNode->position.x, succeedingNode->position.y);

			// Calculate the cost for the new route: g value of the previous node + cost for edge / movement cost to the current node
			auto tentativeG = currentNode->g + edgeValue;

			// If the succeding node is already on the open list but the new route isn't better that the existing one, do nothing
			if (succeedingNode->open && tentativeG >= succeedingNode->g)
				continue;

			// If not -> Set the previous node field and store the g value as the new best value
			succeedingNode->g = tentativeG;
			succeedingNode->previousNode = currentNode;
			succeedingNode->h = succeedingNode->DistanceTo(endPosition);

			// If the succeeding node isn't on the openList yet, add it to the open list
			if (!succeedingNode->open)
			{
				succeedingNode->open = true;
				openList.push(succeedingNode);
			}
		}
	}

	template<class TTileMap>
	inline bool AStarPathfinder<TTileMap>::DoesNodeExist(Position pos) const
	{
		// Access using the formula: y * width + x
		return nodeMap[pos.y * map.GetSize().x + pos.x] != nullptr;
	}

	template<class TTileMap>
	inline void AStarPathfinder<TTileMap>::SetNode(detail::AStarNode::Ptr node)
	{
		// Access using the formula: y * width + x
		nodeMap[node->position.y * map.GetSize().x + node->position.x] = node;
	}

	template<class TTileMap>
	inline detail::AStarNode::Ptr AStarPathfinder<TTileMap>::GetNode(Position pos) const
	{
		// Access using the formula: y * width + x
		return nodeMap[pos.y * map.GetSize().x + pos.x];
	}

#pragma endregion

} // namespace mbe

