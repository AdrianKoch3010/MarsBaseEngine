#pragma once

/// @file
/// @brief Class mbe::Entity and functions mbe::detial::GetComponentID() and mbe::detail::GetComponentTypeID()

#include <string>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <cassert>

#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <MBE/Core/HandleBase.h>
#include <MBE/Core/Component.h>
#include <MBE/Core/Singleton.h>
#include <MBE/Core/Utility.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/ComponentsChangedEvent.h>


namespace mbe
{
	class EntityManager;

	namespace detail
	{
		/// @brief Defines the type of the component id
		/// @details This id is generated at runtime for each component class by calling mbe::detail::GetComponentTypeID()
		typedef std::size_t ComponentTypeID;

		/// @brief Returns a unique ComponentTypeID (for each function call)
		inline ComponentTypeID GetComponentID() noexcept
		{
			// This will only be initialised once
			static ComponentTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		/// @brief Returns a unique number (of type std::size_t) for each type T
		/// @details Each component type will have its own unique id.
		/// The id will be the same for every instance of that type
		/// @tparam T The type for which the id is generated ie classes that inherit from mbe::Component
		template <typename T>
		inline ComponentTypeID GetComponentTypeID() noexcept
		{
			// There will be only one static variable for each template type
			static ComponentTypeID typeId = GetComponentID();
			return typeId;
		}

		class PolyimorphicComponentDictionary : public Singleton<PolyimorphicComponentDictionary>
		{
			friend class Entity;

		private:
			typedef std::unordered_map<ComponentTypeID, std::vector<ComponentTypeID>> PolimorphismDictionary;

		public:
			template <class TDerivedComponent, class TBaseComponent>
			void AddPolymorphism() noexcept
			{
				// Needed since std::is_base_of<T, T> == true
				static_assert(std::is_base_of<Component, TBaseComponent>::value, "PolyimorphicComponentDictionary: TBaseComponent must inherit from Component");
				static_assert(std::is_same<Component, TBaseComponent>::value == false, "PolyimorphicComponentDictionary: TBaseComponent must inherit from Component");
				static_assert(std::is_base_of<TBaseComponent, TDerivedComponent>::value, "PolyimorphicComponentDictionary: TDerivedComponent must inherit from TBaseComponent");
				static_assert(std::is_same<Component, TBaseComponent>::value == false, "PolyimorphicComponentDictionary: TBaseComponent must inherit from Component");

				/// How to make sure that polymorphism isn't added twice?
				baseComponentDictionary[detail::GetComponentTypeID<TDerivedComponent>()].push_back(detail::GetComponentTypeID<TBaseComponent>());
				//derivedComponentDictionary[detail::GetComponentTypeID<TBaseComponent>()].push_back(detail::GetComponentTypeID<TDerivedComponent>());
			}

		private:
			PolimorphismDictionary baseComponentDictionary;
			//PolimorphismDictionary derivedComponentDictionary;
		};

		// Global static variables are internal to the file they are declared in and thus to the files that include it. Hence, another layer of abstraction is needed to add each type only once
		// See: https://stackoverflow.com/questions/11478152/how-to-work-with-variable-in-namespace
		// Not really needed when the MBE_ENABLE_COMPONENT_POLYMORPHISM is used in the .cpp file rather than the header (works since the .cpp is not included, hence only defined once)
		template <class TDerivedComponent, class TBaseComponent>
		struct ComponentPolyimorphismAdderStatic : public detail::Singleton<ComponentPolyimorphismAdderStatic<TDerivedComponent, TBaseComponent>>
		{
			ComponentPolyimorphismAdderStatic() { PolyimorphicComponentDictionary::Instance().AddPolymorphism<TDerivedComponent, TBaseComponent>(); }
		};

	} // namespace detail

	template <class TDerivedComponent, class TBaseComponent>
	struct ComponentPolyimorphismAdder
	{
		// Ensures that the singleton is initialised
		ComponentPolyimorphismAdder() { detail::ComponentPolyimorphismAdderStatic<TDerivedComponent, TBaseComponent>::Instance(); }
	};

} // namespace mbe

/*!
\def MBE_ENABLE_COMPONENT_POLYMORPHISM(derivedComponent, baseComponent)
Enables polymorphic behaviour between the derived and base components

The derivedComponent must inherit from baseComponent and both components must directly or indirectly inherit from mbe::Component.
Multiple inheritance is not supported but chains of inheritance are. For instance, if Derived3 inherits Derived 2 which inherits
from Derived1 and MBE_ENABLE_COMPONENT_POLYMORPHISM(Derived3, Derived2) and MBE_ENABLE_COMPONENT_POLYMORPHISM(Derived2, Derived1)
has been called, then Derived3 and Derived1 also behave polymorphically.
For cases where multiple inheritance would be applicable it should always be possible to split up the derived component into multiple smaller components.
Token concatenation is used to generate the variable name (see https://en.wikipedia.org/wiki/C_preprocessor#Token_concatenation)

\note This macro should be put in the .cpp file of the derived component. Putting it in the .h file still works but is less efficient
since the polymorphism is added every time the header file is included.
*/
#define MBE_ENABLE_COMPONENT_POLYMORPHISM(derivedComponent, baseComponent)														\
namespace mbe																													\
{																																\
	namespace componentPolymorphismDefinitions																					\
	{																															\
		static const ComponentPolyimorphismAdder<derivedComponent, baseComponent> derivedComponent##baseComponent##_variable;	\
	}																															\
}

namespace mbe
{
	/// @brief An Object that can have multiple Components
	/// @details The entity class sits at the heart of the entity component system.
	/// By adding components, state and behaviour can be added dynamically during runtime.
	/// This presents an alternative to the traditional inheritance model in which behaviour is
	/// added through inheriting from a base class such as e.g. game object.
	/// Entities can be added to groups to allow simple and efficient retrieval.
	/// To couple the life time of one entity to another, entities can be attached as child entities.
	/// This also allows to retrieve entities that  logically belong to each other. For instance, a house entity might have
	/// three child entities all with a different set of components. Two of them might have a transfrom, render and collision compoent
	/// acting as some drawable and clickable game object whereas the third might only have a transform and audio component and thus
	/// store the audio information associated with the house entity.
	/// This allows for more flexibility that just adding components to the one instance of the house entity.
	/// @attention The group id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Furthermore <b>only ASCII</b> strings should be used.
	class Entity : public HandleBase<Entity>, private sf::NonCopyable
	{
		/// @brief Enables the entity manager to use the entity's constructor
		/// @details Making the constructor protected is necessarry to ensure that the
		/// mbe::EntityManager is the only place where entities can be created.
		friend class EntityManager;

	public:
		/// @brief The type of the id used to access a group
		typedef std::string Group;

	protected:
		/// @brief Constructor
		/// @param entityManager A reference to the mbe::EntityManager in which this component is created
		Entity(EventManager & eventManager, EntityManager & entityManager);

	public:
		/// @brief Default destructor
		~Entity() = default;


	public:
		/// @brief Updates all attached Components
		/// @details this function is called by the update function of the EntityManager
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime);

		/// @brief Returns whether this enity is still active
		/// @details If it is not active, it is flagged to be deleted and should not be used
		bool IsActive() const { return active; }

		/// @brief Flags this entity and all its child entities to be detroyed
		/// @details When this entity is destroyed it detatches itself automatically from its parent entity.
		void Destroy();

		/// @brief Creates a component and adds it to the component list
		/// @details This should be the only way to create components. Components created elsewhere may not be managed properly.
		/// @tparam arguments The arguments that the created component requires in its constructor.
		/// @attention Note that the parentEntity must not be passed (It is added implicetly by this function)
		/// @returns Returns a reference to the added component which can be used insted of GetComponent<T>()
		template <class TComponent, typename... TArguments>
		TComponent & AddComponent(TArguments&&... arguments);

		template <typename... TComponents, typename... TTuples>
		void AddComponents(TTuples&&... tuples);

		//template <typename... TComponents>
		//void AddConstructedComponents(TComponents&&... components);
		//
		//// Fix remove polymorphism
		/////@brief Removes a component from the entity's component list
		///// @throws std::runntime_error if this entity does not have the requested component.
		///// Therefore, before calling this function, the HasComponent() function should be called
		/////  to make sure that this entity actually has the requested component.
		//template <class TComponent>
		//void RemoveComponent();

		/// @brief Returns a refernce to the requested component
		/// @tparam T The type of the requested component
		/// @throws std::runntime_error if this entity does not have the requested component.
		/// Therefore, before calling this function, the HasComponent() function should be called
		///  to make sure that this entity actually has the requested component.
		template <class TComponent>
		TComponent & GetComponent() const;

		/// @brief Returns true if the Entity has the requested component, flase otherwise
		/// @details If an entity has comonent TComponent, it will also be in the corresponding entity group
		/// @tparam T The type of the requested component
		template <class TComponent>
		bool HasComponent() const;

		/// @brief Looks up whether this entity is the group
		/// @param groupId The id of the group to check. It is not case sensitive so capital letters do not matter. Only use ASCII strings!
		/// @returns True if the entity is in the passed in group, false otherwise
		bool IsInGroup(Group groupId) const;

		/// @brief Adds this entity to the group
		/// @param groupId The group to which this entity is added to
		void AddToGroup(Group groupId);

		/// @brief Removes this entity from the group
		/// @param The group to remove the entity from
		void RemoveFromGroup(Group groupId);

		/// @brief Attaches the passed in entity as a child of this entity
		/// @details Couples the child entity's lifetime with the life time of this entity.
		/// Thus, when this entity gets destroyed all its child entities will get destroyed too.
		/// @param childEntityId The id of the child entity to attach (The entity must exist)
		void AttachChild(HandleID childEntityId);

		/// @brief Removes the passed in entity as a child of this entity
		/// @details Since this the child entity is no longer a child of this entity, their lifetimes are no longer coupled.
		/// @param childEntityId The id of the child entity to remove (The entity must exist)
		/// @throws std::runtime_error if the passed in child entity is not a child of this entity
		void DetatchChild(HandleID childEntityId);

		///@brief Attaches this entity as a child of the passed in parent entity
		/// @details Equivalent to calling parentEntity.AttachChild(this->GetHandleID())
		/// @note To detach from the parent, pass in mbe::Entity::GetNullID(). Passing in an invalid entity id
		/// (one for which no entity exists) has the same effect as passing in mbe::Entity::GetNullID().
		/// @param parentEntityId The id of the parent entity (Can be a null id)
		void SetParentEntityID(HandleID parentEntityId);

		/// @brief Gets the parent entity
		/// @returns The id of the parent entity. If this entity has no parent entity, mbe::Entity::GetNullID() is returned
		inline HandleID GetParentEntityID() const { return parentEntityId; }

		/// @breif Returns all child entities
		/// @returns A list of child entity ids. If this entity has no child entities, an empty list is returned
		inline const std::set<HandleID> & GetChildEntityIDList() const { return childEntityIdList; }

	private:
		void AddPolymorphism(detail::ComponentTypeID typeId, Component::Ptr component);
		// The polymorphic component dictionary must be updated to support this
		//void RemovePolymorphism(detail::ComponentTypeID typeId);
		//void RemoveDerivedComponents(detail::ComponentTypeID typeId);
		//void RemoveBaseComponents(detail::ComponentTypeID typeId);

		// Template method to unpack the tules passed to the AddComponents() method
		template <class TComponent, typename TTuple>
		void AddComponentUnpack(TTuple && tuple);

		//// Add component that has been constructed outside the entiy
		//template <class TComponent>
		//void AddConstructedComponent(std::shared_ptr<TComponent> component);

	private:
		EntityManager & entityManager;
		EventManager & eventManager;

		bool active;
		std::unordered_map<detail::ComponentTypeID, Component::WPtr> componentDictionary;
		std::unordered_map<detail::ComponentTypeID, Component::Ptr> actualComponentDictionary;

		std::vector<Group> groupList; /// <The IDs of the groups that this entity belongs to

		Entity::HandleID parentEntityId;
		std::set<Entity::HandleID> childEntityIdList;
	};


#pragma region Template Implementations

	template <class TComponent, typename... TArguments>
	inline TComponent & Entity::AddComponent(TArguments&&... arguments)
	{
		// Needed since std::is_base_of<T, T> == true
		static_assert(std::is_base_of<Component, TComponent>::value, "Entity: TComponent must inherit from Component");
		static_assert(std::is_same<Component, TComponent>::value == false, "Entity: TComponent must inherit from Component");

		auto typeId = detail::GetComponentTypeID<TComponent>();

		// Make sure that the component doesn't already exist
		assert(this->HasComponent<TComponent>() == false && "Entity: The component already exists");

		auto component = std::make_shared<TComponent>(eventManager, *this, std::forward<TArguments>(arguments)...);

		// For debugging
		std::cout << std::endl << "Entity: Added component with id " << std::to_string(typeId);

		actualComponentDictionary.insert(std::make_pair(typeId, component));
		componentDictionary.insert(std::make_pair(typeId, component));

		entityManager.AddEntityToGroup(*this, typeId);

		this->AddPolymorphism(typeId, component);
		// Tell the engine that the entity's component composition has changed
		eventManager.RaiseEvent(mbe::event::ComponentsChangedEvent(*this));

		return *component;
	}

	template <class TComponent, typename TTuple>
	inline void Entity::AddComponentUnpack(TTuple && tuple)
	{
		std::apply([this](auto&&... args) {
			AddComponent<TComponent>(std::forward<decltype(args)>(args)...);
		}, std::forward<TTuple>(tuple));
	}

	template <typename... TComponents, typename... TTuples>
	inline void Entity::AddComponents(TTuples&&... tuples)
	{
		(AddComponentUnpack<TComponents, TTuples>(std::forward<TTuples>(tuples)), ...);
	}

	//template<class TComponent>
	//inline void Entity::AddConstructedComponent(std::shared_ptr<TComponent> componentPtr)
	//{
	//	// Needed since std::is_base_of<T, T> == true
	//	static_assert(std::is_base_of<Component, TComponent>::value, "Entity: TComponent must inherit from Component");
	//	static_assert(std::is_same<Component, TComponent>::value == false, "Entity: TComponent must inherit from Component");
	//
	//	auto typeId = detail::GetComponentTypeID<TComponent>();
	//
	//	// Make sure that the component doesn't already exist
	//	assert(this->HasComponent<TComponent>() == false && "Entity: The component already exists");
	//
	//	// For debugging
	//	std::cout << std::endl << "Entity: Added constructed component with id " << std::to_string(typeId);
	//
	//	actualComponentDictionary.insert(std::make_pair(typeId, componentPtr));
	//	componentDictionary.insert(std::make_pair(typeId, componentPtr));
	//
	//	entityManager.AddEntityToGroup(*this, typeId);
	//
	//	this->AddPolymorphism(typeId, componentPtr);
	//	// Tell the engine that the entity's component composition has changed
	//	eventManager.RaiseEvent(mbe::event::ComponentsChangedEvent(*this));
	//}
	//
	//template<typename ...TComponents>
	//inline void Entity::AddConstructedComponents(TComponents && ...components)
	//{
	//	// C++17 fold expression
	//	(AddConstructedComponent<TComponents>(std::make_shared<TComponents>(std::forward<TComponents>(components))), ...);
	//}
	//
	//template <class TComponent>
	//inline void Entity::RemoveComponent()
	//{
	//	// Needed since std::is_base_of<T, T> == true
	//	static_assert(std::is_base_of<Component, TComponent>::value, "Entity: TComponent must inherit from Component");
	//	static_assert(std::is_same<Component, TComponent>::value == false, "Entity: TComponent must inherit from Component");
	//
	//	auto typeId = detail::GetComponentTypeID<TComponent>();
	//
	//	if (!this->HasComponent<TComponent>())
	//		throw std::runtime_error("Enity: This entity does not have the requested component Id: " + std::to_string(typeId));
	//
	//	this->RemovePolymorphism(typeId);
	//
	//	actualComponentDictionary.erase(typeId);
	//	componentDictionary.erase(typeId);
	//	// Tell the engine that the entity's component composition has changed
	//	eventManager.RaiseEvent(mbe::event::ComponentsChangedEvent(*this));
	//}

	template <class TComponent>
	inline TComponent & Entity::GetComponent() const
	{
		// Needed since std::is_base_of<T, T> == true
		static_assert(std::is_base_of<Component, TComponent>::value, "Entity: TComponent must inherit from Component");
		static_assert(std::is_same<Component, TComponent>::value == false, "Entity: TComponent must inherit from Component");

		const auto it = componentDictionary.find(detail::GetComponentTypeID<TComponent>());

		// Make sure the component exists
		// Don't use HasComponent() to avoid unnecessary lookup
		if (it == componentDictionary.cend())
			throw std::runtime_error("Enity: This entity does not have the requested component Id: " + std::to_string(detail::GetComponentTypeID<TComponent>()));

		return *std::static_pointer_cast<TComponent>(it->second.lock());
	}

	template <class TComponent>
	inline bool Entity::HasComponent() const
	{
		// Needed since std::is_base_of<T, T> == true
		static_assert(std::is_base_of<Component, TComponent>::value, "Entity: TComponent must inherit from Component");
		static_assert(std::is_same<Component, TComponent>::value == false, "Entity: TComponent must inherit from Component");

		return componentDictionary.find(detail::GetComponentTypeID<TComponent>()) != componentDictionary.end();
	}

#pragma endregion

} // namespace mbe