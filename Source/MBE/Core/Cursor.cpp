#include <MBE/Core/Utility.h>

#include <MBE/Core/Cursor.h>

using namespace mbe;

void Cursor::AttachObject(const ObjectID& objectId)
{
	this->objectId = NormaliseIDString(objectId);
}

void Cursor::DetachObject()
{
	objectId = "";
}

bool Cursor::IsObjectAttached(const ObjectID& objectId) const
{
	return this->objectId == NormaliseIDString(objectId);
}

bool Cursor::IsObjectAttached() const
{
	return objectId != "";
}
