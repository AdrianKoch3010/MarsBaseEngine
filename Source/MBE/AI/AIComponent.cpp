#include <MBE/AI/AIComponent.h>

using namespace mbe;

AIComponent::AIComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{

}

void AIComponent::Refresh()
{
	for (auto it = taskDictionaryList.begin(); it != taskDictionaryList.end(); )
	{
		if (it->second.empty())
			it = taskDictionaryList.erase(it);
		else
			++it;
	}
}

bool AIComponent::HasTask(AITask::HandleID taskId) const
{
	for (const auto & pair : taskDictionaryList)
	{
		if (pair.second.count(taskId))
			return true;
	}

	return false;
}