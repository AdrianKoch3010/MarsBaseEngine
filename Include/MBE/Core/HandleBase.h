#pragma once

/// @file
/// @brief Class template mbe::HandleBase

#include <unordered_map>
#include <memory>
#include <iostream>
#include <limits>

#include <MBE/Core/Exceptions.h>

namespace mbe
{
	template<class TDerived>
	class HandleID;
	class Entity;

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
		//typedef unsigned long long int HandleID;

		friend class HandleID<TDerived>;

	private:
		typedef std::unordered_map<HandleID<TDerived>, TDerived*> HandleDictionary;

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

		/// @brief Finds the object corresponding to the id and returns a pointer to it
		/// @details This function allows for checking an objects existance through comparing 
		/// the returned pointer with a nullptr.
		/// @param id The id of the object to find
		/// @returns A pointer to the found object if it exists, nullptr otherwise
		static TDerived* GetObjectFromID(HandleID<TDerived> id);

		/// @brief Returns an id for which no object exists
		/// @details mbe::HandleBase::GetObjectFromID(mbe::HandleBase::GetNullID()) should always return nullptr.
		/// @returns The largest possible id.
		static const HandleID<TDerived> GetNullID();

	private:
		static HandleID<TDerived> NextHandle();
		static HandleDictionary& GetMap();

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

		this->id = NextHandle();
		GetMap()[id] = static_cast<TDerived*>(this);
		return *this;
	}

	template<class TDerived>
	HandleBase<TDerived>::~HandleBase()
	{
		auto it = GetMap().find(id);
		GetMap().erase(it);
	}

	template<class TDerived>
	inline const HandleID<TDerived>& HandleBase<TDerived>::GetHandleID() const
	{
		return id;
	}

	template<class TDerived>
	inline TDerived* HandleBase<TDerived>::GetObjectFromID(HandleID<TDerived> id)
	{
		auto it = GetMap().find(id);
		if (it == GetMap().end())
			return nullptr;

		return it->second;
	}

	template<class TDerived>
	inline const HandleID<TDerived> HandleBase<TDerived>::GetNullID()
	{
		return { std::numeric_limits<HandleID>::max() };
	}

	template<class TDerived>
	inline typename HandleID<TDerived> HandleBase<TDerived>::NextHandle()
	{
		// Every handle will get its own unique id
		static HandleID<TDerived> next(0);
		return next++;
	}

	template<class TDerived>
	inline typename HandleBase<TDerived>::HandleDictionary& HandleBase<TDerived>::GetMap()
	{
		// Create the static map which will be used to keep track of the Derived handles and their ids
		static HandleDictionary map;
		return map;
	}

#pragma endregion

} // namespace mbe


// Class for handle id --> Just a wrapper around an int, This can now be type safe and preserve const correctness
// (Will have to adapt the event objects as they will no longer be const)
namespace mbe
{
	template <class TDerived>
	class HandleID
	{
	public:
		/// @brief Defines the id type used to uniquely identify a handled object
		/// @details unsigned long long int (uint64_t) is the largest int type in C++.
		/// It allows for 18,446,744,073,709,551,615 unique ids. This is very unlikely to ever be reached.
		/// Note also that the ids will start from 0 for every derived type
		typedef unsigned long long UnderlyingType;

	public:
		HandleID(UnderlyingType id) : id(id) {}

		inline UnderlyingType GetID() const { return id; }

		//static const HandleID Null;

		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
		// This will throw an exception if _DEBUG is enabled
		TDerived& GetExistingObject();

		// Const overload (Important to preserve const correctness)
		const TDerived& GetExistingObject() const;

		// This mimincs the old GetObjectFromID behaviour
		// Returns a nullptr if this id is no longer valid
		inline TDerived* const GetObjectPtr() { return HandleBase<TDerived>::GetObjectFromID(id); }
		inline const TDerived* const GetObjectPtr() const { return HandleBase<TDerived>::GetObjectFromID(id); }


		// Overload dereference and reffering operator for nicer syntax
		inline TDerived& operator*() { return GetExistingObject(); }
		inline const TDerived& operator*() const { return GetExistingObject(); }
		inline TDerived* const operator->() { return &GetExistingObject(); }
		inline const TDerived* const operator->() const { return &GetExistingObject(); }

		// Does exist
		// This returns true if this HandleID is still refering to an active element
		bool Valid() const;

		template <class TDerived>
		friend std::ostream& operator<<(std::ostream& os, const HandleID<TDerived>& id);

		template <class TDerived>
		friend bool operator==(const HandleID<TDerived>& left, const HandleID<TDerived>& right);

		template <class TDerived>
		friend bool operator!=(const HandleID<TDerived>& left, const HandleID<TDerived>& right);

		// Prefix incremeant
		HandleID operator++();

		// Postfix increment
		HandleID operator++(int);

	private:
		UnderlyingType id;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Overload specifically for the Entity class, taking into account the active flag
	template<>
	class HandleID<Entity>
	{
	public:
		typedef unsigned long long UnderlyingType;

	public:
		HandleID(UnderlyingType id);

		// Copy and Move constructor and assignmen operators are need due to the pointer cache
		HandleID(const HandleID& other);
		HandleID(HandleID&& other);

		HandleID& operator=(const HandleID& other);
		HandleID& operator=(HandleID&& other);

		inline UnderlyingType GetUnderlyingID() const { return id; }


		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
		// This will throw an exception if _DEBUG is enabled
		inline Entity& GetExistingEntity();

		// Const overload (Important to preserve const correctness)
		inline const Entity& GetExistingEntity() const;

		// This mimincs the old GetObjectFromID behaviour
		// Returns a nullptr if this id is no longer valid
		inline Entity* const GetEntityPtr() { return HandleBase<Entity>::GetObjectFromID(id); }
		inline const Entity* const GetEntityPtr() const { return HandleBase<Entity>::GetObjectFromID(id); }

		// Overload dereference and reffering operator for nicer syntax
		inline Entity& operator*() { return GetExistingEntity(); }
		inline const Entity& operator*() const { return GetExistingEntity(); }
		inline Entity* const operator->() { return &GetExistingEntity(); }
		inline const Entity* const operator->() const { return &GetExistingEntity(); }

		// This returns true if this HandleID is still refering to an active element
		bool Valid() const;

		////////////////////////////////// -- change to get next id
		// Precremeant operator
		HandleID operator++();

		// Postcremeant operator
		HandleID operator++(int);

		friend std::ostream& operator<<(std::ostream& os, const HandleID& id);
		friend bool operator==(const HandleID& left, const HandleID& right);
		friend bool operator!=(const HandleID& left, const HandleID& right);

	private:
		const UnderlyingType id;
		Entity* pointerCache;
	};

#pragma region Template Implementations

	template<class TDerived>
	TDerived& HandleID<TDerived>::GetExistingObject()
	{
#ifdef _DEBUG
		if (!Valid())
			throw mbe::IDNotFoundException(id);
#endif // _DEBUG
		return *Entity::GetObjectFromID(id);
	}

	template<class TDerived>
	const TDerived& HandleID<TDerived>::GetExistingObject() const
	{
#ifdef _DEBUG
		if (!Valid())
			throw mbe::IDNotFoundException(id);
#endif // _DEBUG
		return *Entity::GetObjectFromID(id);
	}

	template<class TDerived>
	bool HandleID<TDerived>::Valid() const
	{
		return HandleBase<TDerived>::GetObjectFromID(id) != nullptr;
	}

	template<class TDerived>
	std::ostream operator<<(std::ostream& os, const HandleID<TDerived>& id)
	{
		os << id.id;
		return os;
	}

	template<class TDerived>
	bool operator==(const HandleID<TDerived>& left, const HandleID<TDerived>& right)
	{
		return left.id == right.id;
	}

	template<class TDerived>
	bool operator!=(const HandleID<TDerived>& left, const HandleID<TDerived>& right)
	{
		return !(left == right);
	}

#pragma endregion

	typedef HandleID<Entity> EntityID;

} // namespace mbe