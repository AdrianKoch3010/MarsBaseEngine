#include <MBE/AI/AIComponent.h>

using namespace mbe;

//AIComponent::AIComponent(EventManager & eventManager, Entity & parentEntity) :
//	Component(eventManager, parentEntity)
//{
//
//}
//
//void AIComponent::Refresh()
//{
//	for (auto it = taskDictionaryList.begin(); it != taskDictionaryList.end(); )
//	{
//		if (it->second.empty())
//			it = taskDictionaryList.erase(it);
//		else
//			++it;
//	}
//}
//
//bool AIComponent::HasTask(AITask::HandleID taskId) const
//{
//	for (const auto & pair : taskDictionaryList)
//	{
//		if (pair.second.count(taskId))
//			return true;
//	}
//
//	return false;
//}

UtilityAIComponent::UtilityAIComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity)
{

}

bool UtilityAIComponent::IsTaskActive() const
{
	bool active = true;
	if (currentTask.first == nullptr || currentTask.second == std::numeric_limits<detail::AITaskTypeID>::max())
		active = false;

	return active;
}

bool UtilityAIComponent::IsTaskQueued() const
{
	bool active = true;
	if (nextTask.first == nullptr || nextTask.second == std::numeric_limits<detail::AITaskTypeID>::max())
		active = false;

	return active;
}

void UtilityAIComponent::ResetActiveTask()
{
	currentTask.first.reset();
	currentTask.second = detail::UnspecifiedAITaskTypeID;
}

void UtilityAIComponent::ResetQueuedTask()
{
	nextTask.first.reset();
	nextTask.second = detail::UnspecifiedAITaskTypeID;
}

void UtilityAIComponent::MakeNextTaskActive()
{
	if (IsTaskQueued() == false)
		throw std::runtime_error("The current task cannot be replaced since no task is currently queued");

	// Copy the task data
	currentTask.first = std::move(nextTask.first);
	currentTask.second = nextTask.second;

	ResetQueuedTask();
}
