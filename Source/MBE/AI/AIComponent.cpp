#include <MBE/AI/AIComponent.h>

using namespace mbe;

AIComponent::AIComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{

}

void AIComponent::RemoveState(StateID stateId)
{
	NormaliseIDString(stateId);
	stateDictionary.erase(stateId);
}

std::vector<AIComponent::StateID> AIComponent::GetActiveStates() const
{
	std::vector<StateID> activeStates;
	for (const auto & pair : stateDictionary)
		activeStates.push_back(pair.first);

	return activeStates;
}

bool AIComponent::IsStateActive(const StateID & id) const
{
	auto it = stateDictionary.find(id);
	return it != stateDictionary.cend();
}
