#pragma once

/// @file
/// @brief Template class HandleID, HandleID<Entity>, HandleID<const Entity>, and std::hash specialisation

#include <unordered_map>
#include <ostream>
#include <limits>
#include <functional>
#include <type_traits>

#include <MBE/Core/Exceptions.h>

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

	/// @brief Acts as a unique identifier for and smart pointer to an object
	/// @details Provides a wrapper around an unsigned long long base type. For performance reasons, a pointer to the referenced object
	/// is cached. This object can be accessed through the interface provided by this class and const correctness will be preserved.
	/// If the referenced object gets deleted the HandleID will no longer be valid.
	/// This class is the id used by the HandleBase<TDerived> template base class.
	/// @tparam T The type of the object to which the id refers. Note that the class will behave differently for const types.
	/// @see HandleBase, HandleID<Entity> and HandleID<const Entity>
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
		/// @brief Default constructor
		/// @details Initialises the object with null id
		/// @see HandleBase<TDerived>::GetNullID()
		HandleID();

		/// @brief Constructor from an underlying id
		/// @details This constructor is not explicit, so conversion from an underlying type is possible,
		/// i.e. HanldeID<T> id = 42 is valid syntax.
		/// @param id The underlying id of the object
		HandleID(UnderlyingType id);

		/// @brief Copy constructor
		/// @param other Another HandleID of the same type
		HandleID(const HandleID& other);

		/// @brief Copy assigmnet operator
		/// @param other Another HandleID of the same type
		/// @return A reference to this HandleID
		HandleID& operator=(const HandleID& other);

		/// @brief Copy constructor from HandleID<T> where T is not const
		/// @details This enables HandleID<const T> id(HandleID<T>()) but not
		/// HandleID<T> id(HandleID<const T>()).
		/// @note This constructor is only enabled where T is const
		/// @tparam U Same as T if T is const, disabed otherwise
		/// @param other A HandleID of the same template argument not const
		template <typename U = T,
			std::enable_if_t<!std::is_same_v<U, noConstT>, int> = 0>
			HandleID(const HandleID<noConstT>& other);

		/// @brief Copy assignment operator from HandleID<T> where T is not const
		/// @details This enables HandleID<const T> id = HandleID<T>() but not
		/// HandleID<T> id = HandleID<const T>().
		/// @note This assignment operator is only enabled where T is const
		/// @tparam U Same as T if T is const, disabed otherwise
		/// @param other A HandleID of the same template argument not const
		/// @returns A reference to this HandleID
		template <int..., typename U = T,
			std::enable_if_t<!std::is_same_v<U, noConstT>, int> = 0>
			HandleID& operator=(const HandleID<noConstT>& other);

	public:
		/// @brief Returns the underlying id
		inline UnderlyingType GetUnderlyingID() const { return id; }

		/// @brief Gives direct access to the managed object pointer
		/// @return A const pointer to the object or nullptr if it has been deleted
		/// @attention This method exposes a raw pointer to an object that might memory managed elsewhere
		inline T* const GetObjectPtr() const { return GetObjectFromID(id); }

		/// @brief Get an object that is known to still exist
		/// @note If the _DEBUG compiler flag is enabled, an IDNotFound exception will be thrown.
		/// In release mode this behaviour is disabled for performance reasons
		/// @return A reference to the object
		T& GetExistingObject() const;

		/// @brief Overload of the dereference operator for easier object access
		/// @details This is equivalent to GetExistingObject()
		/// @see GetExistingObject()
		/// @return A reference to the object
		inline T& operator*() const { return GetExistingObject(); }

		/// @brief Overload of the reffering operator for easier object access
		/// @details This is equivalent to &GetExistingObject()
		/// @see GetExistingObject()
		/// @return A const pointer to the object
		inline T* const operator->() const { return &GetExistingObject(); }

		/// @brief Returns whether the id is valid
		/// @details Checks if the object has been deleted. This is done through a static dictionary shared
		/// with the HandleBase template base class.
		/// @see HandleID<Entity> and HandleID<const Entity>
		/// @return True if the object still exists, false otherwise
		bool Valid() const;

		/// @brief Enables the id to be printed
		/// @param os The std::ostream to which the underlying id is printed
		/// @param id This id
		/// @return A reference to the std::ostream for chaining
		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<T>& id);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are equal, false otherwise
		friend bool operator== <>(const HandleID<T>& left, const HandleID<T>& right);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are different, false otherwise
		friend bool operator!= <>(const HandleID<T>& left, const HandleID<T>& right);

		/// @brief Enables two ids to be compared
		/// @see std::hash<mbe::HandleID<T>> for use in a std::map
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying id of the left id is less than that of the right, false otherwise
		friend bool operator< <>(const HandleID<T>& left, const HandleID<T>& right);

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

	/// @brief Acts as a unique identifier for and smart pointer to an Entity
	/// @details Provides a wrapper around an unsigned long long base type. For performance reasons, a pointer to the referenced entity
	/// is cached. The entity can be accessed through the interface provided by this class and const correctness will be preserved.
	/// If the entity gets deleted, the HandleID will no longer be valid.
	/// @note This is the id type returned by Entity::GetHandleID() when called on a non-const entity. For a const Entity see HandleID<const Entity>
	/// @see HandleBase, Entity, HandleID<const Entity>
	template<>
	class HandleID<Entity>
	{
		friend class HandleBase<Entity>;
		friend class HandleID<const Entity>;

	public:
		/// @brief Defines the id type used to uniquely identify a handled object
		/// @details unsigned long long int (uint64_t) is the largest int type in C++.
		/// It allows for 18,446,744,073,709,551,615 unique ids. This is very unlikely to ever be reached.
		typedef unsigned long long UnderlyingType;

	private:
		typedef std::unordered_map<UnderlyingType, Entity*> HandleDictionary;

	public:
		/// @brief Default constructor
		/// @details Initialises the object with null id
		/// @see Entity::GetNullID()
		HandleID();

		/// @brief Constructor from an underlying id
		/// @details This constructor is not explicit, so conversion from an underlying type is possible,
		/// i.e. Entity::ID id = 42 is valid syntax.
		/// @param id The underlying id of the object
		HandleID(UnderlyingType id);

		/// @brief Copy constructor
		/// @param other Another HandleID<Entity>
		HandleID(const HandleID& other);

		/// @brief Copy assigmnet operator
		/// @param other Another HandleID<Entity>
		/// @return A reference to this HandleID
		HandleID& operator=(const HandleID& other);

	public:
		/// @brief Returns the underlying id
		inline UnderlyingType GetUnderlyingID() const { return id; }

		/// @brief Gives direct access to the managed entity pointer
		/// @return A const pointer to the entity or nullptr if it has been deleted
		/// @attention This method exposes a raw pointer to an entity that is likely managed by an EntityManager
		// This could be done by using the internal valid function and the chachedPointer --> changes IsActive() behaviour
		inline Entity* const GetEntityPtr() const { return GetEntityFromID(id); }

		/// @brief Get an entity that is known to still exist
		/// @note If the _DEBUG compiler flag is enabled, an IDNotFound exception will be thrown.
		/// In release mode this behaviour is disabled for performance reasons
		/// @return A reference to the entity
		Entity& GetExistingEntity() const;

		/// @brief Overload of the dereference operator for easier access
		/// @details This is equivalent to GetExistingEntity()
		/// @see GetExistingEntity()
		/// @return A reference to the entity
		inline Entity& operator*() const { return GetExistingEntity(); }

		/// @brief Overload of the reffering operator for easier access
		/// @details This is equivalent to &GetExistingEntity()
		/// @see GetExistingEntity()
		/// @return A const pointer to the entity
		inline Entity* const operator->() const { return &GetExistingEntity(); }

		/// @brief Returns whether the id is valid
		/// @details Checks if the entity has been deleted.
		/// This is done through a static dictionary shared with the HandleBase template base class. 
		/// @return True if the entity exists, false otherwise
		bool Valid() const;

		/// @brief Enables the id to be printed
		/// @param os The std::ostream to which the underlying id is printed
		/// @param id This id
		/// @return A reference to the std::ostream for chaining
		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<Entity>& id);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are equal, false otherwise
		friend bool operator== <>(const HandleID<Entity>& left, const HandleID<Entity>& right);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are different, false otherwise
		friend bool operator!= <>(const HandleID<Entity>& left, const HandleID<Entity>& right);

		/// @brief Enables two ids to be compared
		/// @see std::hash<mbe::HandleID<T>> for use in a std::map
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying id of the left id is less than that of the right, false otherwise
		friend bool operator< <>(const HandleID<Entity>& left, const HandleID<Entity>& right);

	private:
		static Entity* GetEntityFromID(const UnderlyingType& id);

		static UnderlyingType NextHandleID();
		static HandleDictionary& GetMap();

	private:
		UnderlyingType id;
		Entity* cachedPointer;
	};

#pragma endregion


#pragma region Template Specialisation for T = const mbe::Entity

	/// @brief Acts as a unique identifier for and smart pointer to a const Entity
	/// @details Provides a wrapper around an unsigned long long base type. For performance reasons, a pointer to the referenced entity
	/// is cached. The entity can be accessed through the interface provided by this class and const correctness will be preserved.
	/// If the entity gets deleted, the HandleID will no longer be valid.
	/// @note This is the id type returned by Entity::GetHandleID() when called on a const entity. For a non-const Entity see HandleID<Entity>
	/// @see HandleBase, Entity, HandleID<Entity>
	template<>
	class HandleID<const Entity>
	{
		friend class HandleBase<const Entity>;

	public:
		/// @brief Defines the id type used to uniquely identify a handled object
		/// @details unsigned long long int (uint64_t) is the largest int type in C++.
		/// It allows for 18,446,744,073,709,551,615 unique ids. This is very unlikely to ever be reached.
		typedef unsigned long long UnderlyingType;

	private:
		typedef std::unordered_map<UnderlyingType, const Entity*> HandleDictionary;

	public:
		/// @brief Default constructor
		/// @details Initialises the object with null id
		/// @see Entity::GetNullID()
		HandleID();

		/// @brief Constructor from an underlying id
		/// @details This constructor is not explicit, so conversion from an underlying type is possible,
		/// i.e. Entity::ConstID id = 42 is valid syntax.
		/// @param id The underlying id of the object
		HandleID(UnderlyingType id);

		/// @brief Copy constructor
		/// @param other Another HandleID<const Entity>
		HandleID(const HandleID<const Entity>& other);

		/// @brief Copy assigmnet operator
		/// @param other Another HandleID<Entity>
		/// @return A reference to this HandleID
		HandleID& operator=(const HandleID<const Entity>& other);

		/// @brief Copy constructor from HandleID<Entity>
		/// @details This enables HandleID<const Entity> id(HandleID<Entity>()) but not
		/// HandleID<Entity> id(HandleID<const Entity>()).
		/// @param other Another handle id of type HandleID<Entity>
		HandleID(const HandleID<Entity>& other);

		/// @brief Copy assignment operator from HandleID<Entity>
		/// @details This enables HandleID<const Entity> id = HandleID<Entity>() but not
		/// HandleID<Entity> id = HandleID<const Entity>().
		/// @param other Another handle id of type HandleID<Entity>
		/// @returns A reference to this HandleID
		HandleID& operator=(const HandleID<Entity>& other);

	public:
		/// @brief Returns the underlying id
		inline UnderlyingType GetUnderlyingID() const { return id; }

		/// @brief Gives direct access to the managed entity pointer
		/// @return A const pointer to the const entity or nullptr if it has been deleted.
		/// @attention This method exposes a raw pointer to an entity that is likely managed by an EntityManager
		// This could be done by using the internal valid function and the chachedPointer --> changes IsActive() behaviour
		inline const Entity* const GetEntityPtr() const { return GetEntityFromID(id); }

		/// @brief Get a const entity that is known to still exist
		/// @note If the _DEBUG compiler flag is enabled, an IDNotFound exception will be thrown.
		/// In release mode this behaviour is disabled for performance reasons
		/// @return A const reference to the entity
		const Entity& GetExistingEntity() const;
		
		/// @brief Overload of the dereference operator for easier access
		/// @details This is equivalent to GetExistingEntity()
		/// @see GetExistingEntity()
		/// @return A const reference to the entity
		inline const Entity& operator*() const { return GetExistingEntity(); }

		/// @brief Overload of the reffering operator for easier access
		/// @details This is equivalent to &GetExistingEntity()
		/// @see GetExistingEntity()
		/// @return A const pointer to the const entity
		inline const Entity* const operator->() const { return &GetExistingEntity(); }

		/// @brief Returns whether the id is valid
		/// @details Checks if the entity has been deleted.
		/// This is done through a static dictionary shared with the HandleBase template base class. 
		/// @return True if the entity exists, false otherwise
		bool Valid() const;

		/// @brief Enables the id to be printed
		/// @param os The std::ostream to which the underlying id is printed
		/// @param id This id
		/// @return A reference to the std::ostream for chaining
		friend std::ostream& operator<< <>(std::ostream& os, const HandleID<const Entity>& id);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are equal, false otherwise
		friend bool operator== <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);

		/// @brief Enables two ids to be compared
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying ids are different, false otherwise
		friend bool operator!= <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);

		/// @brief Enables two ids to be compared
		/// @see std::hash<mbe::HandleID<T>> for use in a std::map
		/// @param left Left operand
		/// @param right Right operand
		/// @return True if the underlying id of the left id is less than that of the right, false otherwise
		friend bool operator< <>(const HandleID<const Entity>& left, const HandleID<const Entity>& right);

	private:
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
	/// @brief Template specialisation of the std::hash function
	/// @tparam T The type of objects refered to by the HandleID
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
