#pragma once

/// @file
/// @brief Class mbe::EntityManager

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <algorithm>

#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <MBE/Core/Entity.h>


namespace mbe
{
	/// @brief Keeps track of a list of entities
	/// @details There should only be a single EntityManager per State
	class EntityManager : private sf::NonCopyable
	{
		/// @brief Enables the entity to access the entity managers AddEntityToGroup() method
		friend class Entity;

	private:
		typedef std::unordered_map<detail::ComponentTypeID, std::vector<Entity::HandleID>> EntityGroupDictionary;

	public:
		/// @brief Default constructor
		EntityManager() = default;

		/// @brief Default Destructor
		~EntityManager() = default;

	public:
		/// @brief Updates all managed entities
		/// @details Should be called once each frame
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime);

		/// @brief Creates a default entity
		/// @details Components can be added later on.
		/// @n The CreateEntity() mehtod is be the only way to create entities since entities
		/// created elsewhere may not be managed properly.
		/// @returns A reference to the created entity
		Entity & CreateEntity();

		/// @brief Creates a derived entity
		/// @details Components may be added in the entities constructor but can also be added later on.
		/// @n The CreateEntity() mehtod is be the only way to create entities since entities
		/// created elsewhere may not be managed properly.
		/// @tparam T The type of entity created. Must be derived from mbe::Entity
		/// @tparam TArguments The argument parameter pack
		/// @param arguments The arguments forwarded to the derived entities constructor
		/// @returns A reference to the base entity. This is sufficient since classes that derive from entity should
		/// not add additional methods anyway.
		// Is overloading based on the template parameters an option?
		template <typename TData, typename... TArguments>
		Entity & CreateEntity(TArguments&&... arguments);

		// Returns a list of all entites that have been added to this group
		// If the group id doesn't exista an empty list is returned
		// The group id is not case sensitive - ASCII only
		const std::vector<Entity::HandleID>& GetGroup(Entity::Group groupId) const;

		// Returns a list of all entities that have component TComponent
		template <class TComponent>
		const std::vector<Entity::HandleID>& GetComponentGroup() const;

	private:
		/// @brief Deletes all inactive entities
		/// @details Entities are set inactive when calling the mbe::Entity::Destroy() method
		void Refresh();

		// This method is private since it should never be called directly
		// If done so the entity manager might add an entity to a group that the entity itself is not added to
		void AddEntityToGroup(const Entity & entity, Entity::Group groupId);

		void AddEntityToGroup(const Entity & entity, detail::ComponentTypeID componentTypeId);

	private:
		std::vector<std::unique_ptr<Entity>> entityList;
		mutable std::map<Entity::Group, std::vector<Entity::HandleID>> entityGroups;
		mutable EntityGroupDictionary entityGroupDictionary;
	};

#pragma region Template Implementations

	template<typename TData, typename ...TArguments>
	inline Entity & EntityManager::CreateEntity(TArguments && ...arguments)
	{
		// The raw new is needed for the entity manager to access the entities protected constructor
		TData * rawEntity = new TData(*this, std::forward<TArguments>(arguments)...);

		// Make a new unique pointer of the base type
		std::unique_ptr<Entity> entity(rawEntity);

		// Add the entity to the default group (before moving it)
		//entity->AddToGroup("");

		// Move its content to the container
		entityList.emplace_back(std::move(entity));
		// The returned reference just for user convenience (note that the unique_ptr entity has already been moved thus its empty)
		return *rawEntity;
		// Implicetly delete the obsolete/empty unique pointer
	}

	template<class TComponent>
	inline const std::vector<Entity::HandleID>& EntityManager::GetComponentGroup() const
	{
		// Make sure that only Components are added as type id keys for the dictionary
		static_assert(std::is_base_of<Component, TComponent>::value, "EntityManager: TComponent must inherit from mbe::Component");
		static_assert(std::is_same<Component, TComponent>::value == false, "EntityManager: TComponent must inherit from mbe::Component");

		// If no entity with this component has been added, the does not exist and an empty std::vector is inserted and returned for that key
		return entityGroupDictionary[detail::GetComponentTypeID<TComponent>()];
	}

#pragma endregion
	
} // namespace mbe


// Remove entity? --> by setting the entity inactive
// Remove by key? --> no, at least not as long as entities don't have their own id
// How to access entities once they have been added --> through GetGroup() or GetEntities() or the entities HandleId
// How to check whether (references to) added entities are still valid --> Maybe use weak pointers
// How to maintain other list of entities e.g. a list of buttons with a certain set of components --> using GetGroup();
// No lists of derived types should be needed since all the functionality is implemented in the components --> Yep, though derived entities are still valid