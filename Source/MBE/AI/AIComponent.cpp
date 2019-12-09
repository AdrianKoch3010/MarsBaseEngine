#include <MBE/AI/AIComponent.h>

using namespace mbe;

UtilityAIComponent::UtilityAIComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity)
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
	currentTask.first = std::move(nextTask.first);
	currentTask.second = nextTask.second;

	ResetQueuedTask();
}
