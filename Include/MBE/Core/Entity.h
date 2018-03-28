#pragma once

/// @file
/// @brief Class mbe::Entity, GetComponentID() and GetComponentTypeID()

#include <string>
#include <vector>
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


namespace mbe
{
	class EntityManager;

	namespace detail
	{
		typedef std::size_t ComponentTypeID;

		/// @brief Returns a unique number (for each function call) of type std::size_t
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
		/// @tparam T The type for which the id is generated
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
				polimorphismDictionary[detail::GetComponentTypeID<TDerivedComponent>()].push_back(detail::GetComponentTypeID<TBaseComponent>());
			}

		private:
			std::unordered_map<size_t, std::vector<size_t>> polimorphismDictionary;
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
		ComponentPolyimorphismAdder() { detail::ComponentPolyimorphismAdderStatic<TDerivedComponent, TBaseComponent>::Instance(); } // Ensures that the singleton is initialised
	};

} // namespace mbe

  // Note that multiple inheritance is not supported and is not meant to be used. For cases where multiple inheritance would be applicable
  // it should always be possible to split up the derived component into multiple smaller components
  // See Token concatenation https://en.wikipedia.org/wiki/C_preprocessor#Token_concatenation

/*!
\def MBE_ENABLE_COMPONENT_POLYMORPHISM(derivedComponent, baseComponent)
Enables polymorphic behaviour between the derived and base components

The derivedComponent must inherit from baseComponent and both component must directly or indirectly inherit from mbe::Component.
Multiple inheritance is not supported but chains of inheritance are. For instance, if Derived3 inherit Derived 2 which inherits
from Derived1 and MBE_ENABLE_COMPONENT_POLYMORPHISM(Derived3, Derived2) and MBE_ENABLE_COMPONENT_POLYMORPHISM(Derived2, Derived1)
has been called, the Derived3 and Derived1 also behave polymorphically.

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
	/// To couple the life time of one entity to another, entities can be attached as child entities to other entities.
	/// This also allows to retrieve entities that  logically belong to each other. For instance, a house entity might have
	/// three child entities all with a different set of components. Two of them might have a transfrom, render and collision compoent
	/// acting as some drawable, clickable game object whereas the third might only have a transform and audio component and thus
	/// store the audio information associated with the house entity.
	/// This allows for more flexibility that just adding components to the one instance of the house entity.
	class Entity : public HandleBase<Entity>, private sf::NonCopyable
	{
		/// @brief Enables the entity manager to use the components constructor
		/// @details Making the constructor protected is necessarry to ensure that the
		/// mbe::EntityManager is the only place where entities can be created.
		friend class EntityManager;

	public:
		/// @brief The type of the id used to access a group
		typedef std::string Group;

	protected:
		/// @brief Constructor
		/// @param entityManager A reference to the mbe::EntityManager in which this component is created
		/// @param eventManager A reference to the mbe::EventManager which can be used to raise, subscribe or unsubscribe to events
		Entity(EntityManager & entityManager);

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

		/// @brief Creates a component and adds it to this entities component list
		/// @details This should be the only way to create components. Components created elsewhere may not be managed properly.
		/// @tparam arguments The arguments that the created component requires in its constructor.
		/// @attention Note that the parentEntity must not be passed (It is added implicetly by this function)
		/// @returns Returns a reference to the added component which can be used insted of GetComponent<T>()
		template <class TComponent, typename... TArguments>
		TComponent & AddComponent(TArguments&&... arguments);

		/// @brief Returns a refernce to the requested component
		/// @tparam T The type of the requested component
		/// @throws std::runntime_error if this entity does not have the requested component.
		/// Therefore, before calling this function, the HasComponent() function should be called
		///  to make sure that this entity actually has the requested component.
		template<class TComponent>
		TComponent & GetComponent() const;

		/// @brief Returns true if the Entity has the requested component, flase otherwise
		/// @tparam T The type of the requested component
		template <class TComponent>
		bool HasComponent() const;

		// The group ids are not case sensitive, ACSII only
		bool IsInGroup(Group groupId) const;
		void AddToGroup(Group groupId);
		void RemoveFromGroup(Group groupId);

		// Couples the child entity's lifetime with the life time of this entity.
		// Thus, when this entity gets destroyed all its child entities will get destroyed as well
		void AttachChild(HandleID childEntityId);

		// Throws if the passed in child entity is not a child of this entity
		void DetatchChild(HandleID childEntityId);

		// Attaches this entity to the parent
		// Equivalent to calling parentEntity.AttachChild(this->GetHandleID())
		// To detach from the parent, pass in Entity::GetNullID()
		// This entity is detatched from its current parent
		void SetParentEntityID(HandleID parentEntityId);

		// If this entity has no parentEntity, mbe::Entity::GetNullID() is returned
		inline HandleID GetParentEntityID() const { return parentEntityId; }

		// If this entity has no child entities, an empty list is returned
		inline const std::set<HandleID> & GetChildEntityIDList() const { return childEntityIdList; }

	private:
		void AddPolymorphism(detail::ComponentTypeID typeId, Component::Ptr component);

	private:
		EntityManager & entityManager;

		bool active;
		std::vector<Component::Ptr> componentList;
		std::unordered_map<detail::ComponentTypeID, Component::WPtr> componentDictionary;

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
		//assert(componentBitSet[detail::GetComponentTypeID<TComponent>()] == false && "The component already exists");
		//assert(componentArray[detail::GetComponentTypeID<TComponent>()] == nullptr && "The component already exists");

		//TComponent * c = new TComponent(*this, std::forward<TArguments>(arguments)...);
		//std::unique_ptr<Component> uPtr{ c };
		//componentList.emplace_back(std::move(uPtr));
		//// set the component * in the array
		//componentArray[detail::GetComponentTypeID<TComponent>()] = c;
		//// set the according component flag to true
		//componentBitSet[detail::GetComponentTypeID<TComponent>()] = true;
		//return *c;

		auto component = std::make_shared<TComponent>(*this, std::forward<TArguments>(arguments)...);

		// For debugging
		std::cout << std::endl << "Entity: Added component with id " << std::to_string(detail::GetComponentTypeID<TComponent>());

		componentList.push_back(component);
		componentDictionary.insert(std::make_pair(typeId, component));

		this->AddPolymorphism(typeId, component);

		return *component;
	}

	template<class TComponent>
	inline TComponent & Entity::GetComponent() const
	{
		//// Should be the same as
		////Component * component = componentArray[getComponentTypeID<T>()];
		//auto ptr(componentArray[detail::GetComponentTypeID<TComponent>()]);

		//if (ptr == nullptr)
		//	throw std::runtime_error("Entity: This entity does not have the requested component");

		const auto it = componentDictionary.find(detail::GetComponentTypeID<TComponent>());

		// Don't use has component to avoid unnecessary lookup
		if (it == componentDictionary.cend())
			throw std::runtime_error("Enity: This entity does not have the requested component Id: " + std::to_string(detail::GetComponentTypeID<TComponent>()));

		return *std::static_pointer_cast<TComponent>(it->second.lock());
	}

	template <class TComponent>
	inline bool Entity::HasComponent() const
	{
		// Performs a lookup in the bitset for the specific type (The id is the position in the bitset)
		//return componentBitSet[detail::GetComponentTypeID<TComponent>()];

		return componentDictionary.find(detail::GetComponentTypeID<TComponent>()) != componentDictionary.end();
	}

#pragma endregion

} // namespace mbe