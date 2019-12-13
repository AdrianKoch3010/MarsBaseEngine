#include <MBE/AI/AIComponent.h>

using namespace mbe;

UtilityAIComponent::UtilityAIComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity),
	currentTask({ nullptr, detail::UnspecifiedAITaskTypeID }),
	nextTask({ nullptr, detail::UnspecifiedAITaskTypeID })
{

}

bool UtilityAIComponent::IsTaskActive() const
{
	bool active = true;
	if (currentTask.first == nullptr || currentTask.second == detail::UnspecifiedAITaskTypeID)
		active = false;

	return active;
}

bool UtilityAIComponent::IsTaskQueued() const
{
	bool active = true;
	if (nextTask.first == nullptr || nextTask.second == detail::UnspecifiedAITaskTypeID)
		active = false;

	return active;
}

AITask& UtilityAIComponent::GetActiveTask()
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

	assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

	return *currentTask.first;
}

const AITask& UtilityAIComponent::GetActiveTask() const
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

	assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

	return *currentTask.first;
}

AITask::Ptr UtilityAIComponent::GetActiveTaskPtr()
{
	return currentTask.first;
}

UtilityAIComponent::AITaskTypeID UtilityAIComponent::GetActiveTaskTypeID() const
{
	// Throw if no task is active
	if (IsTaskActive() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

	assert(currentTask.second != detail::UnspecifiedAITaskTypeID && "UtilityAIComponent: Currently there is no task active");

	return currentTask.second;
}

AITask& UtilityAIComponent::GetQueuedTask()
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

	assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

	return *nextTask.first;
}

const AITask& UtilityAIComponent::GetQueuedTask() const
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

	assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

	return *nextTask.first;
}

AITask::Ptr UtilityAIComponent::GetQueuedTaskPtr()
{
	return nextTask.first;
}

UtilityAIComponent::AITaskTypeID UtilityAIComponent::GetQueuedTaskTypeID() const
{
	// Throw if no task is queued
	if (IsTaskQueued() == false)
		throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

	assert(nextTask.second != detail::UnspecifiedAITaskTypeID && "UtilityAIComponent: Currently there is no task queued");

	return nextTask.second;
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
		throw std::runtime_error("UtilityAITask: The current task cannot be replaced since no task is currently queued");

	// Copy the task data
	currentTask.first = nextTask.first;
	currentTask.second = nextTask.second;

	ResetQueuedTask();
}
