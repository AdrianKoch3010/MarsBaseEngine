#pragma once

/// @file
/// @brief Class mbe::ClickableComponent

#include <set>

#include <SFML/System/Vector2.hpp>

#include "Entity.h"
#include "Component.h"

namespace mbe
{

	class ClickableComponent : public Component
	{
		friend class ClickableSystem;
	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		ClickableComponent(Entity & parentEntity);

		/// @brief  Default destructor
		virtual ~ClickableComponent() = default;

		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
		/// @param frameTime The delta time between two successive frames
		// Could be called by the clickable system
		inline void Update(sf::Time frameTime) override { RemoveExpiredConnectedClickableEntities(); }

		// The position in local coordinates
		virtual bool Contains(const sf::Vector2f & position) const = 0;

		void AddConnectedClickableEntity(Entity::HandleID entityId);

		void RemoveConnectedClickableEntity(Entity::HandleID entityId);

		// The entity ids contained by this list are not guaranteed to exist
		inline const std::set<Entity::HandleID> & GetConnectedClickableIdList() const { return connectedClickableEntityIdList; }

	private:
		// Is this necessarry?
		void RemoveExpiredConnectedClickableEntities();

	private:
		// List of entities for which a mbe::EntityClickedEvent is raised when this entity is clicked
		// make sure that the list only goes one way - to avoid infinite loops
		// The set guarantees that there no entity id can be inserted multiple times.
		std::set<Entity::HandleID> connectedClickableEntityIdList;
	};

} // namespace mbe