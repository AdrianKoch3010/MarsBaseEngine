#pragma once

/// @file
/// @brief Class template mbe::HandleBase

#include <unordered_map>
#include <limits>

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
		/// @brief Defines the id type used to uniquely identify a handled object
		/// @details unsigned long long int (uint64_t) is the largest int type in C++.
		/// It allows for 18,446,744,073,709,551,615 unique ids. This is very unlikely to ever be reached.
		/// Note also that the ids will start from 0 for every derived type
		typedef unsigned long long int HandleID;

	public:
		/// @brief Default constructor
		HandleBase();

		/// @brief Copy constructor
		/// @details A new handle id is generated for the copy
		HandleBase(const HandleBase & other);

		/// @brief Copy assignment operator
		/// @details A new handle id is generated for the copy
		HandleBase & operator = (const HandleBase & other);

		/// @brief Default destructor
		~HandleBase();

		/// @brief Returns the id of this object
		/// @details Note that the ids will start from 0 for every derived type
		const HandleID GetHandleID() const;

		/// @brief Finds the object corresponding to the id and returns a pointer to it
		/// @details This function allows for checking an objects existance through comparing 
		/// the returned pointer with a nullptr.
		/// @param id The id of the object to find
		/// @returns A pointer to the found object if it exists, nullptr otherwise
		static TDerived * GetObjectFromID(HandleID id);

		/// @brief Returns an id for which no object exists
		/// @details mbe::HandleBase::GetObjectFromID(mbe::HandleBase::GetNullID()) should always return nullptr.
		/// @returns The largest possible id.
		static const HandleID GetNullID();

	private:
		static HandleID NextHandle();
		static std::unordered_map<HandleID, TDerived *>& GetMap();

	private:
		const HandleID id; // The id of this (handle) object
	};

#pragma region Template Implementations

	template<class TDerived>
	HandleBase<TDerived>::HandleBase() :
		id(NextHandle())
	{
		// Should always be save
		// asserting the dynamic_cast does not work since no polimorph type is used
		HandleBase::GetMap()[id] = static_cast<TDerived *>(this);
	}

	template<class TDerived>
	inline HandleBase<TDerived>::HandleBase(const HandleBase & other) :
		id(NextHandle())
	{
		HandleBase::GetMap()[id] = static_cast<TDerived *>(this);
	}

	template<class TDerived>
	inline HandleBase<TDerived> & HandleBase<TDerived>::operator=(const HandleBase & other)
	{
		// Check for self assignment
		if (&other == this)
			return *this;

		this->id = NextHandle();
		HandleBase::GetMap()[id] = static_cast<TDerived *>(this);
		return *this;
	}

	template<class TDerived>
	HandleBase<TDerived>::~HandleBase()
	{
		auto it = HandleBase::GetMap().find(id);
		HandleBase::GetMap().erase(it);
	}

	template<class TDerived>
	inline const typename HandleBase<TDerived>::HandleID HandleBase<TDerived>::GetHandleID() const
	{
		return id;
	}

	template<class TDerived>
	inline TDerived * HandleBase<TDerived>::GetObjectFromID(HandleID id)
	{
		auto it = HandleBase::GetMap().find(id);
		if (it == HandleBase::GetMap().end())
			return nullptr;

		return it->second;
	}

	template<class TDerived>
	inline const typename HandleBase<TDerived>::HandleID HandleBase<TDerived>::GetNullID()
	{
		return std::numeric_limits<HandleID>::max();
	}

	template<class TDerived>
	inline typename HandleBase<TDerived>::HandleID HandleBase<TDerived>::NextHandle()
	{
		// Every handle will get its own unique id
		static HandleID next = 0;
		return next++;
	}

	template<class TDerived>
	inline std::unordered_map<typename HandleBase<TDerived>::HandleID, TDerived *>& HandleBase<TDerived>::GetMap()
	{
		// Create the static map which will be used to keep track of the Derived handles and their ids
		static std::unordered_map<HandleID, TDerived *> map;
		return map;
	}

#pragma endregion

} // namespace mbe