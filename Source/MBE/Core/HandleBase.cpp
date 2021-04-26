#include <MBE/Core/HandleBase.h>
#include <MBE/Core/Entity.h>

using namespace mbe;

HandleID<mbe::Entity>::HandleID(UnderlyingType id) :
	id(id)
{
	pointerCache = HandleBase<Entity>::GetObjectFromID(id);
}

mbe::Entity& HandleID<mbe::Entity>::GetExistingEntity()
{
#ifdef _DEBUG
	if (!Valid())
		throw mbe::IDNotFoundException(id);
#endif // _DEBUG
	return *pointerCache;
}

const mbe::Entity& HandleID<mbe::Entity>::GetExistingEntity() const
{
#ifdef _DEBUG
		if (!Valid())
			throw mbe::IDNotFoundException(id);
#endif // _DEBUG
		return *pointerCache;
}

bool HandleID<mbe::Entity>::Valid() const
{
	const auto entityPtr = Entity::GetObjectFromID(id);
	return  entityPtr != nullptr && entityPtr->IsActive() == true;
}

HandleID<mbe::Entity> HandleID<mbe::Entity>::operator++()
{
	HandleID<Entity> tmp(++id);
	return tmp;
}

HandleID<mbe::Entity> HandleID<mbe::Entity>::operator++(int)
{
	HandleID<Entity> tmp(id++);
	return tmp;
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