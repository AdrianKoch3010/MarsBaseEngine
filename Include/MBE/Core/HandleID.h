#pragma once

/// @file
/// @brief Template class HandleID

#include <unordered_map>
#include <ostream>
#include <limits>
#include <functional>
#include <type_traits>

#include <MBE/Core/Exceptions.h>

// Class for handle id --> Just a wrapper around an int, This can now be type safe and preserve const correctness
namespace mbe
{

#pragma region Forward Declarations

	// Forward declare the HandleID template class
	template<class>
	class HandleID;

	// Forward declare the HandleBase template class
	template<class>
	class HandleBase;

	// Forward declare the Entity for the template specialisations
	class Entity;

	template <class T>
	std::ostream& operator<<(std::ostream& os, const HandleID<T>& id);

	template <class T>
	bool operator==(const HandleID<T>& left, const HandleID<T>& right);

	template <class T>
	bool operator!=(const HandleID<T>& left, const HandleID<T>& right);

	template <class T>
	bool operator<(const HandleID<T>& left, const HandleID<T>& right);

#pragma endregion


#pragma region HandleID Template Class

	template <class T>
	class HandleID
	{
		friend class HandleBase<T>;
		friend class HandleID<const T>;

	public:
		/// @brief Defines the id type used to uniquely identify a handled object
		/// @details unsigned long long int (uint64_t) is the largest int type in C++.
		/// It allows for 18,446,744,073,709,551,615 unique ids. This is very unlikely to ever be reached.
		/// Note also that the ids will start from 0 for every derived type
		typedef unsigned long long UnderlyingType;

	private:
		typedef std::unordered_map<UnderlyingType, T*> HandleDictionary;
		typedef std::remove_const_t<T> noConstT;

	public:
		// Default constructor --> Initialises with null id
		HandleID();

		// Non explicit, so conversion from underlying type is allowed
		HandleID(UnderlyingType id);

		HandleID(const HandleID& other);
		HandleID& operator=(const HandleID& other);

		// Only HandleID<const T>(HandleID<T>) is legal
		template <typename U = T,
			std::enable_if_t<!std::is_same_v<U, noConstT>, int> = 0>
			HandleID(const HandleID<noConstT>& other);

		// Only HandleID<const T> = HandleID<T> is legal
		template <int..., typename U = T,
			std::enable_if_t<!std::is_same_v<U, noConstT>, int> = 0>
			HandleID& operator=(const HandleID<noConstT>& other);

		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<T>& id);

		friend bool operator== <>(const HandleID<T>& left, const HandleID<T>& right);

		friend bool operator!= <>(const HandleID<T>& left, const HandleID<T>& right);

		friend bool operator< <>(const HandleID<T>& left, const HandleID<T>& right);

		inline UnderlyingType GetUnderlyingID() const { return id; }

		// This mimincs the old GetObjectFromID behaviour
		// Returns a nullptr if this id is no longer valid
		inline T* const GetObjectPtr() const { return GetObjectFromID(id); }

		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
		// This will throw an exception if _DEBUG is enabled
		T& GetExistingObject() const;

		// Overload dereference and reffering operator for nicer syntax
		inline T& operator*() const { return GetExistingObject(); }
		inline T* const operator->() const { return &GetExistingObject(); }

		// Does exist
		// This returns true if this HandleID is still refering to an active element
		bool Valid() const;

		//static const HandleID& GetNullID();

	private:
		/// @brief Finds the object corresponding to the id and returns a pointer to it
		/// @details This function allows for checking an objects existance through comparing 
		/// the returned pointer with a nullptr.
		/// @param id The id of the object to find
		/// @returns A pointer to the found object if it exists, nullptr otherwise
		static T* GetObjectFromID(const UnderlyingType& id);

		static UnderlyingType NextHandleID();
		static HandleDictionary& GetMap();

	private:
		UnderlyingType id;
		T* cachedPointer;
	};

#pragma endregion
	

#pragma region Template Specialisation for T = mbe::Entity

	template<>
	class HandleID<Entity>
	{
		friend class HandleBase<Entity>;
		friend class HandleID<const Entity>;

	public:
		typedef unsigned long long UnderlyingType;

	private:
		typedef std::unordered_map<UnderlyingType, Entity*> HandleDictionary;

	public:
		//Default constructor --> Initialises with null id
		HandleID();

		// Nonexplicit, so that the EntityID can be created from the underlying type
		HandleID(UnderlyingType id);

		//// Copy constructor and copy assignment operator
		HandleID(const HandleID& other);
		HandleID& operator=(const HandleID& other);

		inline UnderlyingType GetUnderlyingID() const { return id; }


		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
		// This will throw an exception if _DEBUG is enabled
		Entity& GetExistingEntity() const;

		// Const overload (Important to preserve const correctness)
		//const Entity& GetExistingEntity() const;

		// This mimincs the old GetObjectFromID behaviour
		// Returns a nullptr if this id is no longer valid
		// This could be done by using the internal valid function and the chachedPointer
		inline Entity* const GetEntityPtr() const { return GetEntityFromID(id); }
		//inline const Entity* const GetEntityPtr() const { return GetEntityFromID(id); }

		// Overload dereference and reffering operator for nicer syntax
		inline Entity& operator*() const { return GetExistingEntity(); }
		//inline const Entity& operator*() const { return GetExistingEntity(); }
		inline Entity* const operator->() const { return &GetExistingEntity(); }
		//inline const Entity* const operator->() const { return &GetExistingEntity(); }

		// This returns true if this HandleID is still refering to an active element
		bool Valid() const;

		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<Entity>& id);
		friend bool operator== <>(const HandleID<Entity>& left, const HandleID<Entity>& right);
		friend bool operator!= <>(const HandleID<Entity>& left, const HandleID<Entity>& right);
		friend bool operator< <>(const HandleID<Entity>& left, const HandleID<Entity>& right);

		//static const HandleID& GetNullID();

	private:
		/// @brief Finds the object corresponding to the id and returns a pointer to it
		/// @details This function allows for checking an objects existance through comparing 
		/// the returned pointer with a nullptr.
		/// @param id The id of the object to find
		/// @returns A pointer to the found object if it exists, nullptr otherwise
		static Entity* GetEntityFromID(const UnderlyingType& id);

		static UnderlyingType NextHandleID();
		static HandleDictionary& GetMap();

	private:
		UnderlyingType id;
		Entity* cachedPointer;
	};

#pragma endregion


#pragma region Template Specialisation for T = const mbe::Entity

	template<>
	class HandleID<const Entity>
	{
		friend class HandleBase<const Entity>;

	public:
		typedef unsigned long long UnderlyingType;

	private:
		typedef std::unordered_map<UnderlyingType, const Entity*> HandleDictionary;

	public:
		//Default constructor --> Initialises with null id
		HandleID();

		// Nonexplicit, so that the EntityID can be created from the underlying type
		HandleID(UnderlyingType id);

		// Copy and constructors
		HandleID(const HandleID<const Entity>& other);
		HandleID(const HandleID<Entity>& other);

		// Copy assignment operators
		HandleID& operator=(const HandleID<const Entity>& other);
		HandleID& operator=(const HandleID<Entity>& other);


		inline UnderlyingType GetUnderlyingID() const { return id; }


		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
		// This will throw an exception if _DEBUG is enabled
		const Entity& GetExistingEntity() const;

		// This mimincs the old GetObjectFromID behaviour
		// Returns a nullptr if this id is no longer valid
		// This could be done by using the internal valid function and the chachedPointer
		inline const Entity* const GetEntityPtr() const { return GetEntityFromID(id); }
		//inline const Entity* const GetEntityPtr() const { return GetEntityFromID(id); }

		// Overload dereference and reffering operator for nicer syntax
		inline const Entity& operator*() const { return GetExistingEntity(); }
		//inline const Entity& operator*() const { return GetExistingEntity(); }
		inline const Entity* const operator->() const { return &GetExistingEntity(); }
		//inline const Entity* const operator->() const { return &GetExistingEntity(); }

		// This returns true if this HandleID is still refering to an active element
		bool Valid() const;

		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<const Entity>& id);
		friend bool operator== <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);
		friend bool operator!= <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);
		friend bool operator< <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);

		//static const HandleID& GetNullID();

	private:
		/// @brief Finds the object corresponding to the id and returns a pointer to it
		/// @details This function allows for checking an objects existance through comparing 
		/// the returned pointer with a nullptr.
		/// @param id The id of the object to find
		/// @returns A pointer to the found object if it exists, nullptr otherwise
		static const Entity* GetEntityFromID(const UnderlyingType& id);

		static UnderlyingType NextHandleID();
		static HandleDictionary& GetMap();

	private:
		UnderlyingType id;
		const Entity* cachedPointer;
	};

#pragma endregion


#pragma region Template Implementations

	template<class T>
	HandleID<T>::HandleID() :
		id(std::numeric_limits<UnderlyingType>::max()),
		cachedPointer(nullptr)
	{
	}

	template<class T>
	HandleID<T>::HandleID(UnderlyingType id) :
		id(id),
		cachedPointer(GetObjectFromID(id))
	{
	}

	template<class T>
	HandleID<T>::HandleID(const HandleID<T>& other) :
		id(other.id),
		cachedPointer(other.cachedPointer)
	{
	}

	template<class T>
	template <typename U,
		std::enable_if_t<!std::is_same_v<U, std::remove_const_t<T>>, int>>
		HandleID<T>::HandleID(const HandleID<std::remove_const_t<T>>& other) :
		id(other.id),
		cachedPointer(other.cachedPointer)
	{
	}

	template<class T>
	HandleID<T>& HandleID<T>::operator=(const HandleID<T>& other)
	{
		this->id = other.id;
		this->cachedPointer = other.cachedPointer;
		return *this;
	}

	template<class T>
	template<int..., typename U,
		std::enable_if_t<!std::is_same_v<U, std::remove_const_t<T>>, int>>
		HandleID<T>& HandleID<T>::operator=(const HandleID<std::remove_const_t<T>>& other)
	{
		this->id = other.id;
		this->cachedPointer = other.cachedPointer;
		return *this;
	}

	template<class T>
	T& HandleID<T>::GetExistingObject() const
	{
#ifdef _DEBUG
		if (!Valid())
			throw mbe::IDNotFoundException(id);
#endif // _DEBUG
		return *cachedPointer;
	}

	template<class T>
	bool HandleID<T>::Valid() const
	{
		return GetObjectFromID(id) != nullptr;
	}

	template<class T>
	inline typename HandleID<T>::UnderlyingType HandleID<T>::NextHandleID()
	{
		// Every handle will get its own unique id
		static UnderlyingType next = 0;
		return next++;
	}

	template<class T>
	inline typename HandleID<T>::HandleDictionary& HandleID<T>::GetMap()
	{
		// Create the static map which will be used to keep track of the Derived handles and their ids
		static HandleDictionary map;
		return map;
	}

	template<class T>
	inline T* HandleID<T>::GetObjectFromID(const UnderlyingType& id)
	{
		auto it = GetMap().find(id);
		if (it == GetMap().end())
			return nullptr;

		return it->second;
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, const mbe::HandleID<T>& id)
	{
		os << id.id;
		return os;
	}

	template<class T>
	bool operator==(const mbe::HandleID<T>& left, const mbe::HandleID<T>& right)
	{
		return left.id == right.id;
	}

	template<class T>
	bool operator!=(const mbe::HandleID<T>& left, const mbe::HandleID<T>& right)
	{
		return !(left == right);
	}

	template<class T>
	bool operator<(const mbe::HandleID<T>& left, const mbe::HandleID<T>& right)
	{
		return left.id < right.id;
	}

#pragma endregion

} // namespace mbe

#pragma region Hash Function Template Specialisation

namespace std
{

	template<class T>
	struct hash<mbe::HandleID<T>>
	{
		size_t operator()(const mbe::HandleID<T>& key) const;
	};

	template<class T>
	inline size_t hash<mbe::HandleID<T>>::operator()(const mbe::HandleID<T>& key) const
	{
		// This works since the id uniquely identifies an object
		return hash<mbe::HandleID<T>::UnderlyingType>()(key.GetUnderlyingID());
	}

} // namespace std

#pragma endregion
