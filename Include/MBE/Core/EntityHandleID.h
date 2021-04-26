//#pragma once
//
///// @file
///// @brief Template overload HandleID<Entity>
//
//#include <MBE/Core/HandleID.h>
//#include <MBE/Core/Entity.h>
//
//namespace mbe
//{
//	template<>
//	class HandleID<Entity>
//	{
//	public:
//		typedef unsigned long long UnderlyingType;
//
//		friend class HandleBase<Entity>;
//
//	private:
//		typedef std::unordered_map<HandleID<Entity>, Entity*> HandleDictionary;
//
//	public:
//		// Nonexplicit, so that the EntityID can be created from the underlying type
//		HandleID(UnderlyingType id);
//
//		// Copy and Move constructor and assignmen operators are need due to the pointer cache
//		HandleID(const HandleID& other);
//		HandleID(HandleID&& other);
//
//		HandleID& operator=(const HandleID& other);
//		HandleID& operator=(HandleID&& other);
//
//		inline UnderlyingType GetUnderlyingID() const { return id; }
//
//
//		// Get existing entity (This is for the cases where the entiyt must exist, unless there is a bug in the code)
//		// This will throw an exception if _DEBUG is enabled
//		inline Entity& GetExistingEntity();
//
//		// Const overload (Important to preserve const correctness)
//		inline const Entity& GetExistingEntity() const;
//
//		// This mimincs the old GetObjectFromID behaviour
//		// Returns a nullptr if this id is no longer valid
//		// This could be done by using the internal valid function and the chachedPointer
//		inline Entity* const GetEntityPtr() { return HandleBase<Entity>::GetObjectFromID(id); }
//		inline const Entity* const GetEntityPtr() const { return HandleBase<Entity>::GetObjectFromID(id); }
//
//		// Overload dereference and reffering operator for nicer syntax
//		inline Entity& operator*() { return GetExistingEntity(); }
//		inline const Entity& operator*() const { return GetExistingEntity(); }
//		inline Entity* const operator->() { return &GetExistingEntity(); }
//		inline const Entity* const operator->() const { return &GetExistingEntity(); }
//
//		// This returns true if this HandleID is still refering to an active element
//		bool Valid() const;
//
//		friend std::ostream& operator<<(std::ostream& os, const HandleID& id);
//		friend bool operator==(const HandleID& left, const HandleID& right);
//		friend bool operator!=(const HandleID& left, const HandleID& right);
//
//	private:
//		static HandleID NextHandleID();
//		static HandleDictionary& GetMap();
//
//
//	private:
//		const UnderlyingType id;
//		Entity* cachedPointer;
//	};
//
//} // namespace mbe