#pragma once

/// @file
/// @brief Class mbe::AITask

#include <string>
#include <memory>
#include <queue>
#include <map>

#include <MBE/Core/HandleBase.h>
#include <MBE/AI/AIAction.h>

namespace mbe
{

	///@brief Base class for AIStates managed by the mbe::AIComponent
	///@details An mbe::AIState contains the information on an action performed by the an mbe::Entity
	class AITask : public HandleBase<AITask>
	{
	public:
		typedef std::shared_ptr<AITask> Ptr;
		typedef std::weak_ptr<AITask> WPtr;
		typedef std::unique_ptr<AITask> UPtr;

	public:
		///@brief Constructor
		AITask(float utility);

		///@brief Default Destructor
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

		/// @brief Set the utility of the task
		// @details The utility is a meassure of how important this task currently is.
		/// This is a float value between 0 and 1
		inline void SetUtilty(float value) { utility = value; }

		/// @brief Flags the task as aborting
		inline void SetAborting(bool value = true) { aborting = value; }

		/// @brief Flags the class as completed
		inline void SetCompleted(bool value = true) { completed = value; }
	
	protected:
		float utility;
		bool aborting;
		bool completed;
	};

} // namespace mbe