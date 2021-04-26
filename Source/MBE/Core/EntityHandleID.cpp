#include <MBE/Core/HandleID.h>
#include <MBE/Core/Entity.h>

using namespace mbe;

HandleID<mbe::Entity>::HandleID(UnderlyingType id) :
	id(id),
	cachedPointer(GetEntityFromID(id))
{
}

mbe::Entity& HandleID<mbe::Entity>::GetExistingEntity()
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *cachedPointer;
}

const mbe::Entity& HandleID<mbe::Entity>::GetExistingEntity() const
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *cachedPointer;
}

bool HandleID<mbe::Entity>::Valid() const
{
	const auto entityPtr = GetEntityFromID(id);
	return  entityPtr != nullptr && entityPtr->IsActive() == true;
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