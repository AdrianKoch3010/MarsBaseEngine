#pragma once

/// @file
/// @brief Class template mbe::HandleBase

#include <ostream>
#include <limits>

#include <MBE/Core/HandleID.h>

// Entity -> HandleBase -> HandleID -> Entity

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
		const HandleID<TDerived>& GetHandleID() const;

		/// @brief Returns an id for which no object exists
		/// @details mbe::HandleBase::GetObjectFromID(mbe::HandleBase::GetNullID()) should always return nullptr.
		/// @returns The largest possible id.
		static const HandleID<TDerived> GetNullID();

	private:
		const HandleID<TDerived> id; // The id of this (handle) object
	};

#pragma region Template Implementations

	template<class TDerived>
	HandleBase<TDerived>::HandleBase() :
		id(NextHandle())
	{
		// Should always be save
		// asserting the dynamic_cast does not work since no polimorph type is used
		GetMap()[id] = static_cast<TDerived*>(this);
	}

	template<class TDerived>
	inline HandleBase<TDerived>::HandleBase(const HandleBase& other) :
		id(NextHandle())
	{
		HandleBase::GetMap()[id] = static_cast<TDerived*>(this);
	}

	template<class TDerived>
	inline HandleBase<TDerived>& HandleBase<TDerived>::operator=(const HandleBase& other)
	{
		// Check for self assignment
		if (&other == this)
			return *this;

		this->id = HandleID<TDerived>::NextHandleID();
		HandleID<TDerived>::GetMap()[id] = static_cast<TDerived*>(this);
		return *this;
	}

	template<class TDerived>
	HandleBase<TDerived>::~HandleBase()
	{
		auto it = HandleID<TDerived>::GetMap().find(id);
		HandleID<TDerived>::GetMap().erase(it);
	}

	template<class TDerived>
	inline const HandleID<TDerived>& HandleBase<TDerived>::GetHandleID() const
	{
		return id;
	}

	template<class TDerived>
	inline const HandleID<TDerived> HandleBase<TDerived>::GetNullID()
	{
		return { std::numeric_limits<HandleID>::max() };
	}

	//template<class TDerived>
	//inline typename HandleID<TDerived> HandleBase<TDerived>::NextHandle()
	//{
	//	// Every handle will get its own unique id
	//	static HandleID<TDerived> next(0);
	//	return next++;
	//}

	//template<class TDerived>
	//inline typename HandleBase<TDerived>::HandleDictionary& HandleBase<TDerived>::GetMap()
	//{
	//	// Create the static map which will be used to keep track of the Derived handles and their ids
	//	static HandleDictionary map;
	//	return map;
	//}

#pragma endregion

} // namespace mbe