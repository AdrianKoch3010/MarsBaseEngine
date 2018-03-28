#pragma once

/// @file
/// @brief Class mbe::ParallelTask

#include <functional>
#include <thread>
#include <mutex>

namespace mbe
{
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