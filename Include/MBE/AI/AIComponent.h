#pragma once
#pragma once

/// @file
/// @brief Class mbe::AIComponent

#include <memory>
#include <map>
#include <limits>
//#include <list>

#include <MBE/Core/Component.h>
#include <MBE/AI/AITask.h>

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

		typedef std::size_t AIActionTypeID;

		constexpr AIActionTypeID UnspecifiedAIActionTypeID = std::numeric_limits<AIActionTypeID>::max();

		inline AIActionTypeID GetAIActionID() noexcept
		{
			// This will only be initialised once
			static AIActionTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		template <typename T>
		inline AIActionTypeID GetAIActionTypeID() noexcept
		{
			// There will be only one static variable for each template type
			static AIActionTypeID typeId = GetAIActionID();
			return typeId;
		}

	} //namespace detail

	// Some other system (probably inheriting from AISystem will determine the currently active task)
	// The AIComponent stores the queued tasks and the currently active task
	// Tasks contain data on the action performed by the entity
	////////////////////////////////////////////////////////////////////////// Possible redesigns
	/// Make it possible to remove tasks whoes types are not known
	/// Use handle id to store active task and make the task dictionary use unique ptr
	/// Makes sense depending on how the type based stuff is implemented
	//class AIComponent : public Component
	//{
	//public:
	//	typedef std::shared_ptr<AIComponent> Ptr;
	//	typedef std::weak_ptr<AIComponent> WPtr;
	//	typedef std::unique_ptr<AIComponent> UPtr;
	//
	//	// Dictionary of all tasks of one type
	//	template <class TTask>
	//	using TaskOfOneTypeDictionary = std::map<typename TTask::HandleID, std::shared_ptr<TTask>>;
	//
	//	// Dictionary of all tasks
	//	typedef std::map<detail::AITaskTypeID, TaskOfOneTypeDictionary<AITask>> TaskDictionary;
	//
	//public:
	//	AIComponent(EventManager& eventManager, Entity& parentEntity);
	//	~AIComponent() = default;
	//
	//public:
	//	// Removes empty dictionaries
	//	void Refresh();
	//
	//	// Returns nullptr if no taska are registred
	//	AITask::Ptr GetHighstUtilityTask();
	//
	//	// Create and add a new AITask
	//	// Get the id by using get handle id on the task
	//	template<class TTask, typename... TArguments>
	//	TTask& AddTask(TArguments&& ... arguments);
	//
	//	// Throws if the state doesn't exist
	//	template <class TTask>
	//	void RemoveTask(AITask::HandleID taskID);
	//
	//	// Returns true if any task of this type is queued
	//	template <class TTask>
	//	bool HasTaskType() const;
	//
	//	template <class TTask>
	//	bool HasTask(AITask::HandleID taskId) const;
	//
	//	// Inefficient (has to search linearly) shouldn't be used
	//	bool HasTask(AITask::HandleID taskId) const;
	//
	//	// Either takes handle id or AITast::Ptr
	//	void SetActiveTask(AITask::Ptr taskPtr);
	//
	//	template <class TTask>
	//	bool IsTaskTypeActive();
	//
	//	// nullptr if no task is active
	//	template <class TTask>
	//	std::shared_ptr<TTask> GetActiveTask();
	//
	//	// Const overload
	//	template <class TTask>
	//	const std::shared_ptr<TTask> GetActiveTask() const;
	//
	//	inline AITask::Ptr GetActiveTask() { return activeTask; }
	//
	//	// Const overload
	//	const AITask::Ptr GetActiveTask() const { return activeTask; }
	//
	//	// Throws if no task of this type is registered
	//	// Use HasTaskType to check
	//	// Get all the tasks of one type
	//	template <class TTask>
	//	TaskOfOneTypeDictionary<TTask>& GetTaskDictionary();
	//
	//	// Const overload
	//	template <class TTask>
	//	const TaskOfOneTypeDictionary<TTask>& GetTaskDictionary() const;
	//
	//private:
	//	TaskDictionary taskDictionaryList;
	//	AITask::Ptr activeTask;
	//};
	//
	// There is never more than one task / action active at a time
	// The exception is when a task is aborting, the next task must wait until the aborted task is completed


	class UtilityAIComponent : public Component
	{
	public:
		typedef std::shared_ptr<UtilityAIComponent> Ptr;
		typedef std::weak_ptr<UtilityAIComponent> WPtr;
		typedef std::unique_ptr<UtilityAIComponent> UPtr;

	public:
		UtilityAIComponent(EventManager& eventManager, Entity& parentEntity);
		~UtilityAIComponent() = default;

	public:
		// Sets the current task aborting (if existing)
		// Creates the new task
		template<class TTask, typename... TArguments>
		void AddTask(TArguments&& ...arguments);

		// Check whether there is a next task using IsTaskQueued
		// Throws if no task is queued
		// Moves the next task 
		void MakeNextTaskActive();

		//template<class TAction, typename... TArguments>
		//void ReplaceAction(TArguments&& ...arguments);

		// Returns true if a task of type TTask is active
		template<class TTask>
		bool IsTaskActive() const;

		// Returns true if a task of type TTask is queued
		template<class TTask>
		bool IsTaskQueued() const;

		// Returns true if any task is active
		bool IsTaskActive() const;

		// Returns true if any task is queued
		bool IsTaskQueued() const;

		//template<class TAction>
		//bool IsActionActive() const;

		// Use IsTaskActive before
		// Throws if the task is not active
		template<class TTask>
		TTask& GetActiveTask();

		template<class TTask>
		const TTask& GetActiveTask() const;

		// Use IsTaskQueued before
		// Throws if the task is not queued
		template<class TTask>
		TTask& GetQueuedTask();

		// const overload
		template<class TTask>
		const TTask& GetQueuedTask() const;

		// Use IsActionActive before
		// Throws if the action is not active
		/// Maybe return handle id
		//template<class AIAction>
		//AIAction& GetActiveAction();

		// Const overload
		//template<class AIAction>
		//const AIAction& GetActiveAction() const;

		// Sets the task pointer to nullptr and the task type id to unspecified
		void ResetActiveTask();

		// Sets the task pointer to nullptr and the task type id to unspecified
		void ResetQueuedTask();

		//void ResetActiveAction();

	private:
		// If no task is assigned, the task pointer = nullptr and task type id = max value
		std::pair<AITask::UPtr, detail::AITaskTypeID> currentTask;
		std::pair<AITask::UPtr, detail::AITaskTypeID> nextTask;
		std::pair<AIAction::UPtr, detail::AIActionTypeID> currentAction;
	};

//#pragma region AI Component Template Implementations
//
//	template<class TTask, typename ...TArguments>
//	inline TTask& AIComponent::AddTask(TArguments&& ...arguments)
//	{
//		auto typeId = detail::GetAITaskTypeID<TTask>();
//		auto taskPtr = std::make_shared<TTask>(std::forward<TArguments>(arguments)...);
//		// Creates new dictionary if needed
//		auto taskId = taskPtr->GetHandleID(); // This is guaranteed to be unique
//		taskDictionaryList[typeId].insert(std::make_pair(taskId, taskPtr));
//
//		return *taskDictionaryList.at(typeId).at(taskId);
//	}
//
//	template<class TTask>
//	inline void AIComponent::RemoveTask(AITask::HandleID taskId)
//	{
//		auto it = taskDictionaryList.find(detail::GetAITaskTypeID<TTask>());
//		if (it == taskDictionaryList.end())
//			throw std::runtime_error("AIComponent: This entity does not have the requested task type");
//
//		auto& typeDictonary = it->second;
//		if (typeDictonary.count(taskId) == 0)
//			throw std::runtime_error("AIComponent: This entity does not have the requested task");
//
//		typeDictonary.erase(taskId);
//	}
//
//	template<class TTask>
//	inline bool AIComponent::HasTaskType() const
//	{
//		return taskDictionaryList.count(detail::GetAITaskTypeID<TTask>());
//	}
//
//	template<class TTask>
//	inline bool AIComponent::HasTask(AITask::HandleID taskId) const
//	{
//		// Find the low type dictionary
//		auto it = taskDictionaryList.find(detail::GetAITaskTypeID<TTask>());
//		if (it != taskDictionaryList.end())
//			return it->second.count(taskId);
//
//		return false;
//	}
//
//	template<class TTask>
//	inline std::shared_ptr<TTask> AIComponent::GetActiveTask()
//	{
//		return std::static_pointer_cast<TTask>(activeTask);
//	}
//
//	template<class TTask>
//	inline const std::shared_ptr<TTask> AIComponent::GetActiveTask() const
//	{
//		return std::static_pointer_cast<TTask>(activeTask);
//	}
//
//	template<class TTask>
//	inline typename AIComponent::TaskOfOneTypeDictionary<TTask>& AIComponent::GetTaskDictionary()
//	{
//		// Don't create a new dictionary for this type if it doesn't exist
//		auto typeId = detail::GetAITaskTypeID<TTask>();
//		if (taskDictionaryList.count(typeId) == 0)
//			throw std::runtime_error("AIComponent: This entity does not have the requested task type");
//
//		return taskDictionaryList.at(typeId);
//	}
//
//	template<class TTask>
//	inline const typename AIComponent::TaskOfOneTypeDictionary<TTask>& AIComponent::GetTaskDictionary() const
//	{
//		// Don't create a new dictionary for this type if it doesn't exist
//		auto typeId = detail::GetAITaskTypeID<TTask>();
//		if (taskDictionaryList.count(typeId) == 0)
//			throw std::runtime_error("AIComponent: This entity does not have the requested task type");
//
//		return taskDictionaryList.at(typeId);
//	}
//
//#pragma endregion

#pragma region Utility AI Component Template Implementations

	template<class TTask, typename ...TArguments>
	inline void UtilityAIComponent::AddTask(TArguments&& ...arguments)
	{
		auto taskPtr = std::make_unique<TTask>(std::forward<TArguments>(arguments)...);

		// Where does the task live? 

	}

	template<class TTask>
	inline bool UtilityAIComponent::IsTaskActive() const
	{
		// Return true if a task of that type is active
		auto taskTypeId = detail::GetAITaskTypeID<TTask>();
		return currentTask.second == taskTypeId;
	}

	template<class TTask>
	inline bool UtilityAIComponent::IsTaskQueued() const
	{
		// Return true if a task of that type is queued
		auto taskTypeId = detail::GetAITaskTypeID<TTask>();
		return nextTask.second == taskTypeId;
	}


	template<class TTask>
	inline TTask& UtilityAIComponent::GetActiveTask()
	{
		// Throw if the task doesn't exist
		if (IsTaskActive<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

		assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

		return *static_cast<TTask*>(currentTask.first.get());
	}

	template<class TTask>
	inline const TTask& UtilityAIComponent::GetActiveTask() const
	{
		// Throw if the task doesn't exist
		if (IsTaskActive<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

		assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

		return *static_cast<TTask*>(currentTask.first.get());
	}

	template<class TTask>
	inline TTask& UtilityAIComponent::GetQueuedTask()
	{
		// Throw if the task doesn't exist
		if (IsTaskQueued<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

		assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

		return *nextTask.first;
	}

	template<class TTask>
	inline const TTask& UtilityAIComponent::GetQueuedTask() const
	{
		// Throw if the task doesn't exist
		if (IsTaskQueued<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

		assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

		return *nextTask.first;
	}

#pragma endregion


} //namespace mbe