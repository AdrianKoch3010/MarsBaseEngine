#pragma once

/// @file
/// @brief class mbe::Cursor

#include <memory>
#include <string>

namespace mbe
{

	class Cursor
	{
	public:
		typedef std::shared_ptr<Cursor> Ptr;
		typedef std::weak_ptr<Cursor> WPtr;
		typedef std::unique_ptr<Cursor> UPtr;

		typedef std::string ObjectID;

	public:
		void AttachObject(const ObjectID& objectId);

		void DetachObject();

		inline const ObjectID& GetAttachedObject() const { return objectId; }

		bool IsObjectAttached(const ObjectID& objectId) const;

		bool IsObjectAttached() const;

	private:
		ObjectID objectId;
	};


} // namespace mbe