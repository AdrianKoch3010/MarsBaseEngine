#include <MBE/Core/Entity.h>
#include <MBE/Core/EntityManager.h>

using namespace mbe;


Entity::Entity(EventManager & eventManager, EntityManager & entityManager) :
	eventManager(eventManager),
	entityManager(entityManager),
	active(true),
	parentEntityId(GetNullID())
{
}

void Entity::Update(sf::Time frameTime)
{
	for (auto & pair : actualComponentDictionary)
		pair.second->Update(frameTime);
}

void Entity::Destroy()
{
	this->active = false;

	// If it has a parent entity, detach itself
	const auto parentEntityPtr = Entity::GetObjectFromID(this->parentEntityId);
	if (parentEntityPtr != nullptr)
		parentEntityPtr->DetatchChild(this->GetHandleID());

	// Calling Destroy() changes the childEntityIdList by detatching itself
	// Hence, the set is copied before before iterating it and therefore changing the original
	auto childEntityIdListCopy = childEntityIdList;
	for (const auto & childEntityId : childEntityIdListCopy)
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "Entity: The child entity must exist");
		Entity::GetObjectFromID(childEntityId)->Destroy();
	}
}

bool Entity::IsInGroup(Group groupId) const
{
	NormaliseIDString(groupId);

	// Return whether the groupId has been found in the list
	return std::find(groupList.begin(), groupList.end(), groupId) != groupList.end();
}

void Entity::AddToGroup(Group groupId)
{
	NormaliseIDString(groupId);

	groupList.push_back(groupId);
	entityManager.AddEntityToGroup(*this, groupId);
}

void Entity::RemoveFromGroup(Group groupId)
{
	NormaliseIDString(groupId);

	// Find the groupId in the list
	auto it = std::find(groupList.begin(), groupList.end(), groupId);
	// And remove it from the list
	groupList.erase(it);
}

void Entity::AttachChild(HandleID childEntityId)
{
	// Make sure the entity exists
	assert(Entity::GetObjectFromID(childEntityId) != nullptr && "Entity: The child entity must exists");
	auto & childEntity = *Entity::GetObjectFromID(childEntityId);

	// Set the parentEntity on the child entity
	childEntity.parentEntityId = this->GetHandleID();

	// Add the child entity to the list of child entities
	childEntityIdList.insert(std::move(childEntityId));
}

void Entity::DetatchChild(HandleID childEntityId)
{
	// Make sure the entity exists
	assert(Entity::GetObjectFromID(childEntityId) != nullptr && "Entity: The child entity must exists");
	auto & childEntity = *Entity::GetObjectFromID(childEntityId);

	// Set the child entity's parent to null
	childEntity.parentEntityId = Entity::GetNullID();

	// Remove the child entity from this entity's child entity list
	childEntityIdList.erase(childEntityId);
}

void Entity::SetParentEntityID(HandleID parentEntityId)
{
	// If the new equals the current parent, do nothing
	if (this->parentEntityId == parentEntityId)
		return;

	// Remove this entity from the current parent, if there is one
	auto * currentParentPtr = Entity::GetObjectFromID(this->parentEntityId);
	if (currentParentPtr != nullptr)
		currentParentPtr->DetatchChild(this->GetHandleID());

	// Attatch this to the new parent (if it exists)
	auto * newParentPtr = Entity::GetObjectFromID(parentEntityId);
	if (newParentPtr == nullptr)
		return;

	newParentPtr->AttachChild(this->GetHandleID());
}

// Recursive since a derived class may have multiple base components
// E.g. DerivedComponent3 inherits from DerivedComponent2 which inherits from DerivedComponent1 which inherits from mbe::Component
// If DerivedComponent1 declares a purely virtual function that is implemented in DerivedComponent3, this function should be executed when getting DerivedComponent1
void Entity::AddPolymorphism(detail::ComponentTypeID typeId, Component::Ptr component)
{
	// Find the list of base component ids
	auto it = detail::PolyimorphicComponentDictionary::Instance().baseComponentDictionary.find(typeId);

	// if no base components have been found
	if (it == detail::PolyimorphicComponentDictionary::Instance().baseComponentDictionary.end())
		return;

	// For each base component type id in the list of base component type ids for this component
	for (const auto baseComponentTypeId : it->second)
	{
		// Add polymorphism for the base component's base components
		AddPolymorphism(baseComponentTypeId, component);

		// Make sure that the base component doesn't exist yet. This may be the case if another derived component
		// (inheriting from the same base component) has already been added.
		assert(componentDictionary.find(baseComponentTypeId) == componentDictionary.end() && "Entity: The base component already exists");
		componentDictionary.insert(std::make_pair(baseComponentTypeId, component));

		// Add it to the entity group for this component type
		entityManager.AddEntityToGroup(*this, baseComponentTypeId);
	}
}

//void Entity::RemovePolymorphism(detail::ComponentTypeID typeId)
//{
//	RemoveDerivedComponents(typeId);
//	RemoveBaseComponents(typeId);
//}
//
//void Entity::RemoveDerivedComponents(detail::ComponentTypeID typeId)
//{
//	// Find the list of derived component ids
//	auto it = detail::PolyimorphicComponentDictionary::Instance().derivedComponentDictionary.find(typeId);
//
//	// if no derived components have been found
//	if (it == detail::PolyimorphicComponentDictionary::Instance().derivedComponentDictionary.end())
//		return;
//
//	// By definition there can only be one derived component
//	bool found = false;
//	const auto & derivedComponentList = it->second;
//	// The derived component (if any) that is part of this entity
//	for (auto derivedIt = derivedComponentList.begin(); derivedIt != derivedComponentList.end() && !found; ++derivedIt)
//	{
//		auto derivedTypeId = *derivedIt;
//		if (componentDictionary.count(derivedTypeId))
//		{
//			RemoveDerivedComponents(derivedTypeId);
//			componentDictionary.erase(derivedTypeId);
//			found = true;
//		}
//	}
//
//	/////////////////////////////////////////////////////////////// Remove from entity Group?
//	// Add it to the entity group for this component type
//	//entityManager.AddEntityToGroup(*this, baseComponentTypeId);
//}
//
//void Entity::RemoveBaseComponents(detail::ComponentTypeID typeId)
//{
//	// Find the list of base component ids
//	auto it = detail::PolyimorphicComponentDictionary::Instance().baseComponentDictionary.find(typeId);
//
//	// if no base components have been found
//	if (it == detail::PolyimorphicComponentDictionary::Instance().baseComponentDictionary.end())
//		return;
//
//	// For each base component type id in the list of base component type ids for this component
//	for (const auto baseComponentTypeId : it->second)
//	{
//		// Remove polymorphism for the base component's base components
//		RemoveBaseComponents(baseComponentTypeId);
//
//		// Make sure that the base component exists.
//		// This should never be the case as the AddPolymorphism function permits it
//		// However, in a corrupted case where another component with the same base component exists, this may be possible
//		assert(componentDictionary.find(baseComponentTypeId) != componentDictionary.end() && "Entity: The base component doesn't exists");
//		componentDictionary.erase(baseComponentTypeId);
//
//		/////////////////////////////////////////////////////////////// Remove from entity Group?
//		// Add it to the entity group for this component type
//		//entityManager.AddEntityToGroup(*this, baseComponentTypeId);
//	}
//}
