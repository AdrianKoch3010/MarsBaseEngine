#pragma once

/// @file
/// @brief Class mbe::PathfindingParallelTask

#include <memory>
#include <vector>
#include <chrono>

#include <MBE/Core/ParallelTask.h>
#include <MBE/Core/AStarPathfinder.h>
#include <MBE/Map/TileMapBase.h>

namespace mbe
{

	template <class TTileMap>
	class PathfindingParallelTask : public ParallelTask
	{
	public:
		typedef std::shared_ptr<PathfindingParallelTask<TTileMap>> Ptr;
		typedef std::weak_ptr<PathfindingParallelTask<TTileMap>> WPtr;
		typedef std::unique_ptr<PathfindingParallelTask<TTileMap>> UPtr;

		typedef typename TTileMap::Position Position;

	public:
		
		PathfindingParallelTask(const TTileMap & tileMap,  Position from, Position to);

		// Join the treads
		~PathfindingParallelTask();

	public:
		std::vector<Position> GetPath() const;

	private:
		std::vector<Position> path;

		const TTileMap & tileMap;
		Position from;
		Position to;
	};

#pragma region Template Implementations

	template<class TTileMap>
	inline PathfindingParallelTask<TTileMap>::PathfindingParallelTask(const TTileMap & tileMap, Position from, Position to) : 
		ParallelTask(),
		tileMap(tileMap),
		from(std::move(from)),
		to(std::move(to))

	{
		threadFunction = [this]() {
			//sf::Clock clock;
			AStarPathfinder<TTileMap> pathfinder(this->tileMap);
			path = pathfinder.FindPath(this->from, this->to);
			//std::cout << std::endl << "The pathfinder insertion took " << clock.restart().asMicroseconds() << " us";

			//std::this_thread::sleep_for(std::chrono::seconds(1));

			/*for (size_t i = 0; i < 10000000; i++)
				auto a = i * i + std::sqrt(i);*/
			
			//this->completed may be accessed from multiple threads, protect it
			{
				std::lock_guard<std::mutex> lock(mutex);
				completed = true;
			}
		};

		//threadFunction = []() {};
	}

	template<class TTileMap>
	inline PathfindingParallelTask<TTileMap>::~PathfindingParallelTask()
	{
		if (thread.joinable())
			thread.join();
	}

	template<class TTileMap>
	inline std::vector<typename PathfindingParallelTask<TTileMap>::Position> PathfindingParallelTask<TTileMap>::GetPath() const
	{
		// Use std::move (can only be called once now)
		return path;
	}

#pragma endregion

} // namespace mbe