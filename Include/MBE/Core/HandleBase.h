#pragma once

/// @file
/// @brief Class template mbe::HandleBase

#include <ostream>
#include <limits>

#include <MBE/Core/HandleID.h>

namespace mbe
{
	/// @brief Creates an id for the derived object and provides functionality to access it through an id
	template <class TDerived>
	class HandleBase
		// Make doxygen ignore the abstract definition
		/// @cond
		abstract
		/// @endcond
	{
	public:
		typedef HandleID<const TDerived> ConstID;
		typedef HandleID<TDerived> ID;

	public:
		/// @brief Default constructor
		HandleBase();

		/// @brief Copy constructor
		/// @details A new handle id is generated for the copy
		HandleBase(const HandleBase& other);

		/// @brief Copy assignment operator
		/// @details A new handle id is generated for the copy
		HandleBase& operator= (const HandleBase& other);

		/// @brief Default destructor
		~HandleBase();

	public:
		/// @brief Returns the id of this object
		/// @details Note that the ids will start from 0 for every derived type
		//mbe::HandleID<TDerived>& GetHandleID();

		/// @brief Returns the id of this object
		/// @details Const overload
		/// @details Note that the ids will start from 0 for every derived type
		const HandleID<TDerived>& GetHandleID();

		// Const overload (can't be a reference - The automatic conversion creates a new object)
		HandleID<const TDerived> GetHandleID() const;

		/// @brief Returns an id for which no object exists
		/// @details mbe::HandleBase::GetObjectFromID(mbe::HandleBase::GetNullID()) should always return nullptr.
		/// @returns The largest possible id.
		static const HandleID<TDerived> GetNullID();

	protected:
		// Can't be const because of the constructor
		// But only a const reference is exposed
		HandleID<TDerived> id; // The id of this (handle) object
	};

#pragma region Template Implementations

	template<class TDerived>
	HandleBase<TDerived>::HandleBase()
		// id({ HandleID<TDerived>::NextHandleID() })		-- Should be after the map allocation
	{
		// Should always be save
		// asserting the dynamic_cast does not work since no polimorph type is used
		const auto& nextId = HandleID<TDerived>::NextHandleID(); // Underlying id
		HandleID<TDerived>::GetMap()[nextId] = static_cast<TDerived*>(this);
		id = { nextId }; // Must be after the id has been added to the map (So that the map lookup caches the correct pointer)
	}

	template<class TDerived>
	inline HandleBase<TDerived>::HandleBase(const HandleBase& other)
		//id(HandleID<TDerived>::NextHandleID())
	{
		//HandleID<TDerived>::GetMap()[id] = static_cast<TDerived*>(this);
		const auto& nextId = HandleID<TDerived>::NextHandleID(); // Underlying id
		HandleID<TDerived>::GetMap()[nextId] = static_cast<TDerived*>(this);
		id = { nextId };
	}

	template<class TDerived>
	inline HandleBase<TDerived>& HandleBase<TDerived>::operator=(const HandleBase& other)
	{
		// Check for self assignment
		if (&other == this)
			return *this;

		const auto& nextId = HandleID<TDerived>::NextHandleID();
		HandleID<TDerived>::GetMap()[nextId] = static_cast<TDerived*>(this);
		this->id = { nextId };
		return *this;
	}

	template<class TDerived>
	HandleBase<TDerived>::~HandleBase()
	{
		auto it = HandleID<TDerived>::GetMap().find(id.GetUnderlyingID());
		HandleID<TDerived>::GetMap().erase(it);
	}

	template<class TDerived>
	inline const HandleID<TDerived>& HandleBase<TDerived>::GetHandleID()
	{
		return id;
	}

	template<class TDerived>
	inline HandleID<const TDerived> HandleBase<TDerived>::GetHandleID() const
	{
		return id;
	}

	template<class TDerived>
	inline const HandleID<TDerived> HandleBase<TDerived>::GetNullID()
	{
		return { std::numeric_limits<HandleID<TDerived>::UnderlyingType>::max() };
	}

#pragma endregion

} // namespace mbe