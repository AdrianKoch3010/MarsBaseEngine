#include <MBE/Core/EntityManager.h>

using namespace mbe;

EntityManager::EntityManager(EventManager & eventManager) :
	eventManager(eventManager)
{
}

void EntityManager::Update()
{
	this->Refresh();
}

std::vector<Entity::HandleID> EntityManager::GetEntityIDList() const
{
	std::vector<Entity::HandleID> entityIdList;
	for (const auto& entityPtr : entityList)
	{
		if (entityPtr != nullptr && entityPtr->IsActive())
			entityIdList.push_back(entityPtr->GetHandleID());
	}
	return entityIdList;
}

void EntityManager::Refresh()
{
	// Loops through group
	for (auto & pair : entityGroups)
	{
		auto & groupedEntityList = pair.second;
		groupedEntityList.erase(
			std::remove_if(groupedEntityList.begin(), groupedEntityList.end(),
				[&](Entity::HandleID entityId)
		{
			// Get the entity
			// The entity will still exist since the only way to delete it would be to set it
			// inactive (make sure that inactive entities are deleted after this loop)
			const auto & entity = *Entity::GetObjectFromID(entityId);

			// Remove entities that are:
			// Either marked to be deleted (IsActive() == false)
			// or has been removed from the group (IsInGroup(groupId) == false)
			return !entity.IsActive() || !entity.IsInGroup(pair.first);
		}
		), groupedEntityList.end());
	}

	// Loops through Component Groups
	for (auto & pair : entityGroupDictionary)
	{
		auto & groupedEntityList = pair.second;
		groupedEntityList.erase(
			std::remove_if(groupedEntityList.begin(), groupedEntityList.end(),
				[&](Entity::HandleID entityId)
		{
			// Get the entity
			// The entity will still exist since the only way to delete it would be to set it
			// inactive (make sure that inactive entities are deleted after this loop)
			const auto & entity = *Entity::GetObjectFromID(entityId);

			// Remove entities that are marked to be deleted
			// Entities can't be removed from the group since components can only be added
			return !entity.IsActive();
		}
		), groupedEntityList.end());
	}

	// Loops through every entity and removes it if its inactive
	entityList.erase(std::remove_if(std::begin(entityList), std::end(entityList), [](const std::unique_ptr<Entity> &entity)
	{
		return !entity->IsActive();
	}),
		std::end(entityList));
}

Entity & EntityManager::CreateEntity()
{
	// The raw new is needed for the entity manager to access the entities protected constructor
	Entity * rawEntity = new Entity(eventManager, *this);
	// Make a new unique pointer
	std::unique_ptr<Entity> entity(rawEntity);

	// Add the entity to the default group (before moving it)
	//entity->AddToGroup("");

	// Move its content to the container
	entityList.emplace_back(std::move(entity));
	// The returned reference just for user convenience (note that the unique_ptr entity has already been moved thus its empty)
	return *rawEntity;
	// Implicetly delete the obsolete/empty unique pointer
}

void EntityManager::AddEntityToGroup(const Entity & entity, Entity::Group groupId)
{
	NormaliseIDString(groupId);

	entityGroups[groupId].push_back(entity.GetHandleID());
}

void EntityManager::AddEntityToGroup(const Entity & entity, detail::ComponentTypeID componentTypeId)
{
	entityGroupDictionary[componentTypeId].push_back(entity.GetHandleID());
}

const std::vector<Entity::HandleID>& EntityManager::GetGroup(Entity::Group groupId) const
{
	NormaliseIDString(groupId);

	//const auto & it = entityGroups.find(groupId);

	// No since then a temporary value is returned
	//// If the group id has not been found, an emtpy list is returned
	//// This is better than using the [] operator since no emtpy vector is inserted into the dictionary for nonsense keys
	//if (it == entityGroups.cend())
	//	return std::vector<Entity::HandleID>();

	//return it->second;

	return entityGroups[groupId];
}