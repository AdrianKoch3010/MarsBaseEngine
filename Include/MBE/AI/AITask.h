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
		inline float GetUtility() const { return utility; }

		inline bool IsAborting() const { return aborting; }

		inline bool IsCompleted() const { return completed; }

		inline void SetUtilty(float value) { utility = value; }

		inline void SetAborting(bool value = true) { aborting = value; }

		inline void SetCompleted(bool value = true) { completed = value; }
	
	protected:
		float utility;
		bool aborting;
		bool completed;
	};

	// Sequences task
	// Has a queue of actions with a corresponding state
	// Progresses from one state to another
	class SequencedAITask : public AITask
	{
	public:
		typedef std::shared_ptr<SequencedAITask> Ptr;
		typedef std::weak_ptr<SequencedAITask> WPtr;
		typedef std::unique_ptr<SequencedAITask> UPtr;

		// How about type?
		typedef std::queue<typename AIAction::Ptr> ActionQueue;

	public:
		SequencedAITask(float utility) : AITask(utility) {};
		~SequencedAITask() = default;

	public:

	private:
		ActionQueue actionQueue;
	};

} // namespace mbe