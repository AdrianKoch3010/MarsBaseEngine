#pragma once

/// @file
/// @brief Class mbe::AITask

#include <memory>

//#include <MBE/Core/HandleBase.h>
#include <MBE/Map/TileMapBase.h>

namespace mbe
{

	namespace detail
	{
		typedef std::size_t AITaskTypeID;

		constexpr AITaskTypeID UnspecifiedAITaskTypeID = std::numeric_limits<AITaskTypeID>::max();

		inline AITaskTypeID GetAITaskID() noexcept
		{
			// This will only be initialised once
			static AITaskTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		template <typename T>
		inline AITaskTypeID GetAITaskTypeID() noexcept
		{
			// There will be only one static variable for each template type
			static AITaskTypeID typeId = GetAITaskID();
			return typeId;
		}

	} //namespace detail

	/// @brief Base class for AIStates managed by the mbe::AIComponent
	/// @details An mbe::AIState contains the information on an action performed by the an mbe::Entity
	class AITask /*: public HandleBase<AITask>*/
	{
	public:
		typedef std::shared_ptr<AITask> Ptr;
		typedef std::weak_ptr<AITask> WPtr;
		typedef std::unique_ptr<AITask> UPtr;

	public:
		class Data
		{
		public:
			Data(detail::AITaskTypeID typeId, AITask::Ptr taskPtr) : typeId(typeId), taskPtr(taskPtr) {}

			inline detail::AITaskTypeID GetTypeID() const { return typeId; }
			inline AITask::Ptr GetTaskPtr() const { return taskPtr; }

			// Operator overloads
			inline bool operator<(const Data& taskData) const { return taskPtr < taskData.taskPtr; }
			inline bool operator==(const Data& taskData) const { return taskPtr == taskData.taskPtr; }

		private:
			detail::AITaskTypeID typeId;
			AITask::Ptr taskPtr;
		};

	public:
		/// @brief Constructor
		AITask(float utility);

		/// @brief Default Destructor
		virtual ~AITask() = default;

	public:
		/// @brief Returns the utility of the task
		/// @details The utility is a meassure of how important this task currently is.
		/// This is a float value between 0 and 1
		inline float GetUtility() const { return utility; }

		/// @brief Returns true if the task is currently aborting, false otherwise
		inline bool IsAborting() const { return aborting; }

		/// @brief Returns true if the task is completed, false otherwise
		inline bool IsCompleted() const { return completed; }

		/// @brief Returns true if the task has failed, false otherwise
		/// @details A task task can still complete even if it failed
		inline bool HasFailed() const { return failed; }

		/// @brief Set the utility of the task
		// @details The utility is a meassure of how important this task currently is.
		/// This is a float value between 0 and 1
		/// Set to false by default
		inline void SetUtilty(float value) { utility = value; }

		/// @brief Flags the task as aborting
		/// @details Set to false by default
		inline void SetAborting(bool value = true) { aborting = value; }

		/// @brief Flags the task as completed
		inline void SetCompleted(bool value = true) { completed = value; }

		/// @brief Flags the task as failed
		inline void SetFailed(bool value = true) { failed = value; }
		
		/// @brief Return the task's starting position on the map
		virtual const mbe::TileMapBase::Position& GetStartingPosition() const = 0;
	
	protected:
		float utility;
		bool aborting;
		bool completed;
		bool failed;
	};

} // namespace mbe