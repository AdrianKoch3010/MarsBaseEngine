#include <MBE/AI/AIComponent.h>

using namespace mbe;

AIComponent::AIComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity),
	currentTask({ nullptr, detail::UnspecifiedAITaskTypeID }),
	nextTask({ nullptr, detail::UnspecifiedAITaskTypeID })
{

}

bool AIComponent::IsTaskActive() const
{
	bool active = true;
	if (currentTask.first == nullptr || currentTask.second == detail::UnspecifiedAITaskTypeID)
		active = false;

	return active;
}

bool AIComponent::IsTaskQueued() const
{
	bool active = true;
	if (nextTask.first == nullptr || nextTask.second == detail::UnspecifiedAITaskTypeID)
		active = false;

	return active;
}

AITask& AIComponent::GetActiveTask()
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("AIComponent: Currently there is no task active");

	assert(currentTask.first != nullptr && "AIComponent: Currently there is no task active");

	return *currentTask.first;
}

const AITask& AIComponent::GetActiveTask() const
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("AIComponent: Currently there is no task active");

	assert(currentTask.first != nullptr && "AIComponent: Currently there is no task active");

	return *currentTask.first;
}

AITask::Ptr AIComponent::GetActiveTaskPtr()
{
	return currentTask.first;
}

AIComponent::AITaskTypeID AIComponent::GetActiveTaskTypeID() const
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("AIComponent: Currently there is no task active");

	assert(currentTask.second != detail::UnspecifiedAITaskTypeID && "AIComponent: Currently there is no task active");

	return currentTask.second;
}

AITask& AIComponent::GetQueuedTask()
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("AIComponent: Currently there is no task queued");

	assert(nextTask.first != nullptr && "AIComponent: Currently there is no task queued");

	return *nextTask.first;
}

const AITask& AIComponent::GetQueuedTask() const
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("AIComponent: Currently there is no task queued");

	assert(nextTask.first != nullptr && "AIComponent: Currently there is no task queued");

	return *nextTask.first;
}

AITask::Ptr AIComponent::GetQueuedTaskPtr()
{
	return nextTask.first;
}

AIComponent::AITaskTypeID AIComponent::GetQueuedTaskTypeID() const
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("AIComponent: Currently there is no task queued");

	assert(nextTask.second != detail::UnspecifiedAITaskTypeID && "AIComponent: Currently there is no task queued");

	return nextTask.second;
}

void AIComponent::ResetActiveTask()
{
	currentTask.first.reset();
	currentTask.second = detail::UnspecifiedAITaskTypeID;
}

void AIComponent::ResetQueuedTask()
{
	nextTask.first.reset();
	nextTask.second = detail::UnspecifiedAITaskTypeID;
}

void AIComponent::SetActiveTask(typename AITask::Ptr taskPtr, typename AIComponent::AITaskTypeID typeId)
{
	currentTask.first = taskPtr;
	currentTask.second = typeId;
}

void AIComponent::SetQueuedTask(typename AITask::Ptr taskPtr, typename AIComponent::AITaskTypeID typeId)
{
	nextTask.first = taskPtr;
	nextTask.second = typeId;
}

void AIComponent::MakeNextTaskActive()
{
	if (IsTaskQueued() == false)
		throw std::runtime_error("UtilityAITask: The current task cannot be replaced since no task is currently queued");

	// Copy the task data
	currentTask.first = nextTask.first;
	currentTask.second = nextTask.second;

	ResetQueuedTask();
}
