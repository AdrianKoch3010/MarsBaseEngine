#include <MBE/Core/HandleID.h>
#include <MBE/Core/Entity.h>

using namespace mbe;

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

Entity& HandleID<Entity>::GetExistingEntity()
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *cachedPointer;
}

const Entity& HandleID<Entity>::GetExistingEntity() const
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
	return  entityPtr != nullptr && entityPtr->IsActive() == true;
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


// Not sure if these are needed or if they are taken care off by the template versions

//std::ostream& operator<<(std::ostream& os, const HandleID<Entity>& id)
//{
//	os << id.id;
//	return os;
//}
//
//bool operator==(const HandleID<Entity>& left, const HandleID<Entity>& right)
//{
//	return left.id == right.id;
//}
//
//bool operator!=(const HandleID<Entity>& left, const HandleID<Entity>& right)
//{
//	return !(left == right);
//}
//
//bool operator<(const HandleID<Entity>& left, const HandleID<Entity>& right)
//{
//	return left.id < right.id;
//}