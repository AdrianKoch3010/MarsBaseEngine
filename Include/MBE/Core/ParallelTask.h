#pragma once

/// @file
/// @brief Class mbe::ParallelTask

#include <functional>
#include <thread>
#include <mutex>

#include <iostream>
#include <SFML/System/Clock.hpp>

namespace mbe
{
	// When this->completed is set to true by an inheriting class
	class ParallelTask
	{
	public:
		ParallelTask();
		virtual ~ParallelTask() = default;

		virtual void Execute();
		inline bool IsCompleted() const { return completed; }

	protected:
		std::thread thread;
		mutable std::mutex mutex;

		std::function<void()> threadFunction;
		bool completed;
	};

} // namespace mbe