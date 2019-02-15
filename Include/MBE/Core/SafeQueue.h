#pragma once

/// @file
/// @brief Template class mbe::SafeQueue

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace mbe
{

	template <typename T>
	class SafeQueue
	{
	public:
		SafeQueue();
		~SafeQueue() = default;

	public:
		void Push(T item);

		//void Push(T && item);

		void ShutDown();

		// Throws if the underlying std::queue is empty
		T Pop();

		// Waits until a there is something to fetch
		T TryPop();

		bool IsEmpty() const;

	private:
		std::queue<T> queue;
		mutable std::mutex mutex;
		std::condition_variable conditionVariable;
		std::atomic_bool empty;
		std::atomic_bool running;
	};

#pragma region Template Implementations

	template<typename T>
	inline SafeQueue<T>::SafeQueue() :
		empty(true),
		running(true)
	{
	}

	template<typename T>
	inline void SafeQueue<T>::Push(T item)
	{
		std::lock_guard lock(mutex);
		queue.push(std::move(item));
		empty = false;
		conditionVariable.notify_one();
	}

	/*template<typename T>
	inline void SafeQueue<T>::Push(T && item)
	{
		std::lock_guard lock(mutex);
		queue.push(std::move(item));
		empty = false;
		conditionVariable.notify_one();
	}*/

	template<typename T>
	inline void SafeQueue<T>::ShutDown()
	{
		std::lock_guard lock(mutex);
		running = false;
		conditionVariable.notify_all();
	}

	template<typename T>
	inline T SafeQueue<T>::Pop()
	{
		// Same as 
		auto result = std::move(queue.front());
		queue.pop();

		// Update the empty flag
		if (queue.empty())
			empty = true;

		return result;
	}

	template<typename T>
	inline T SafeQueue<T>::TryPop()
	{
		std::unique_lock lock(mutex);
		while (empty && running)
			conditionVariable.wait(lock);

		if (!running)
			return T();

		// If done correctly the queue must be non empty at this point
		auto result = std::move(queue.front());
		queue.pop();

		// Update the empty flag
		if (queue.empty())
			empty = true;

		return result;
	}

	template<typename T>
	inline bool SafeQueue<T>::IsEmpty() const
	{
		std::lock_guard lock(mutex);
		return queue.empty();
	}

#pragma endregion

} // namespace mbe