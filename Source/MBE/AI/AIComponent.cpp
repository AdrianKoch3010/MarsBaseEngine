#include <MBE/AI/AIComponent.h>

using namespace mbe;

AIComponent::AIComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{

}

void AIComponent::Update(sf::Time frameTime)
{
	for (auto & pair : stateDictionary)
	{
		auto & state = *pair.second;
		state.Update();
	}
}