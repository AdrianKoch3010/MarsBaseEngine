#include <MBE/Core/HandleID.h>
#include <MBE/Core/Entity.h>

using namespace mbe;

#pragma region Template Specialisation for T = mbe::Entity

HandleID<Entity>::HandleID() :
	id(std::numeric_limits<UnderlyingType>::max()),
	cachedPointer(nullptr)
{
}

HandleID<Entity>::HandleID(UnderlyingType id) :
	id(id),
	cachedPointer(GetEntityFromID(id))
{
}

HandleID<Entity>::HandleID(const HandleID<Entity>& other) :
	id(other.id),
	cachedPointer(other.cachedPointer)
{
}

HandleID<Entity>& HandleID<Entity>::operator=(const HandleID<Entity>& other)
{
	this->id = other.id;
	this->cachedPointer = other.cachedPointer;
	return *this;
}

Entity& HandleID<Entity>::GetExistingEntity() const
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *cachedPointer;
}

bool HandleID<Entity>::Valid() const
{
	const auto entityPtr = GetEntityFromID(id);
	return entityPtr != nullptr;
	//return  entityPtr != nullptr && entityPtr->IsActive() == true;
}

inline Entity* HandleID<Entity>::GetEntityFromID(const UnderlyingType& id)
{
	auto it = GetMap().find(id);
	if (it == GetMap().end())
		return nullptr;

	return it->second;
}

HandleID<Entity>::UnderlyingType HandleID<Entity>::NextHandleID()
{
	// Every handle will get its own unique id
	static UnderlyingType next = 0;
	return next++;
}

HandleID<Entity>::HandleDictionary& HandleID<Entity>::GetMap()
{
	// Create the static map which will be used to keep track of the Derived handles and their ids
	static HandleDictionary map;
	return map;
}

#pragma endregion


#pragma region Template Specialisation for T = const mbe::Entity

HandleID<const Entity>::HandleID() :
	id(std::numeric_limits<UnderlyingType>::max()),
	cachedPointer(nullptr)
{
}

HandleID<const Entity>::HandleID(UnderlyingType id) :
	id(id),
	cachedPointer(GetEntityFromID(id))
{
}

HandleID<const Entity>::HandleID(const HandleID<const Entity>& other) :
	id(other.id),
	cachedPointer(other.cachedPointer)
{
}

HandleID<const Entity>& HandleID<const Entity>::operator=(const HandleID<const Entity>& other)
{
	this->id = other.id;
	this->cachedPointer = other.cachedPointer;
	return *this;
}

// Creates a ID<const Entity> from a ID<Entity>
HandleID<const Entity>::HandleID(const HandleID<Entity>& other) :
	id(other.id),
	cachedPointer(other.cachedPointer)
{
}

// Assigns a ID<Entity> to a ID<const Entity>
HandleID<const Entity>& HandleID<const Entity>::operator=(const HandleID<Entity>& other)
{
	this->id = other.id;
	this->cachedPointer = other.cachedPointer;
	return *this;
}

const Entity& HandleID<const Entity>::GetExistingEntity() const
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *cachedPointer;
}

bool HandleID<const Entity>::Valid() const
{
	const auto entityPtr = GetEntityFromID(id);
	return entityPtr != nullptr;
	//return  entityPtr != nullptr && entityPtr->IsActive() == true;
}

inline const Entity* HandleID<const Entity>::GetEntityFromID(const UnderlyingType& id)
{
	auto it = GetMap().find(id);
	if (it == GetMap().end())
		return nullptr;

	return it->second;
}

HandleID<const Entity>::UnderlyingType HandleID<const Entity>::NextHandleID()
{
	// Every handle will get its own unique id
	static UnderlyingType next = 0;
	return next++;
}

HandleID<const Entity>::HandleDictionary& HandleID<const Entity>::GetMap()
{
	// Create the static map which will be used to keep track of the Derived handles and their ids
	static HandleDictionary map;
	return map;
}

#pragma endregion
