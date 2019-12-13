#include <MBE/AI/AISystem.h>

using namespace mbe;

BaseAISystem::BaseAISystem(EntityManager& entityManager, EventManager& eventManager) :
	entityManager(entityManager),
	eventManager(eventManager)
{

}
