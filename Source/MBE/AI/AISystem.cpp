#include <MBE/AI/AISystem.h>

using namespace mbe;

AISystem::AISystem(EntityManager & entityManager) :
	entityManager(entityManager)
{

}

void AISystem::Update()
{
	// Choose the task with the highest utility as the next task

	// Maybe have a value for each task that determines by how much its utility rises when it becomes active

	for (const auto entityId : entityManager.GetComponentGroup<AIComponent>())
	{
		auto & entity = *Entity::GetObjectFromID(entityId);
		auto & aiComponent = entity.GetComponent<AIComponent>();

		// Refresh the ai component
		aiComponent.Refresh();

		auto currentlyActiveTask = aiComponent.GetActiveTask();
		auto highestUtilityTask = aiComponent.GetHighstUtilityTask();

		// If the task will change and there is currently an active task
		if (currentlyActiveTask != nullptr && currentlyActiveTask != highestUtilityTask)
		{
			// Allow the current task to abort its actions if needed
			// This may take a while, aborting tasks should have highest priority
			//	--> add identifyer is aborting
			currentlyActiveTask->SetAborting();
		}

		// if current task is not aborting
		if (currentlyActiveTask != nullptr && currentlyActiveTask->IsAborting() == false)
		{
			/// remove current task?
			//aiComponent.RemoveTask(currentlyActiveTask->GetHandleID());
			aiComponent.SetActiveTask(highestUtilityTask);
		}

		/// If going with the more OOP stype approach 
		// Update the currently active task
		if (aiComponent.GetActiveTask() != nullptr)
		{
			if (aiComponent.GetActiveTask()->IsAborting())
				aiComponent.GetActiveTask()->Abort();
			else
				aiComponent.GetActiveTask()->Update();
		}
	}
}