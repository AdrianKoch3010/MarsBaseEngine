#include <MBE/AI/AIComponent.h>

using namespace mbe;

AIComponent::AIComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{

}

void AIComponent::RemoveState(StateID stateId)
{
	NormaliseIDString(stateId);
	std::remove_if(stateDictionary.begin(), stateDictionary.end(), [&](const decltype(*stateDictionary.begin()) & pair)
	{
		return pair.first == stateId;
	});
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
	auto it = std::find(stateDictionary.cbegin(), stateDictionary.cend(), NormaliseIDString(id));
	return it != stateDictionary.cend();
}
