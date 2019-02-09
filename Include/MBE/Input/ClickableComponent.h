#pragma once

/// @file
/// @brief Class mbe::ClickableComponent

#include <set>

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{

	class ClickableComponent : public Component
	{
		friend class ClickableSystem;
	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		ClickableComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief  Default destructor
		virtual ~ClickableComponent() = default;

		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
		/// @param frameTime The delta time between two successive frames
		// Could be called by the clickable system
		//inline void Update(sf::Time frameTime) override { RemoveExpiredConnectedClickableEntities(); }

		// The position in local coordinates
		virtual bool Contains(const sf::Vector2f & position) const = 0;

		//void AddConnectedClickableEntity(Entity::HandleID entityId);

		//void RemoveConnectedClickableEntity(Entity::HandleID entityId);

		// The entity ids contained by this list are not guaranteed to exist
		// The parent entity is always part of that list
		//inline const std::set<Entity::HandleID> & GetConnectedClickableIdList() const { return connectedClickableEntityIdList; }

		inline void SetAbsorbeClick(bool value = true) { absorbeClick = value; }

		// if bubble up if set to true (in the Clickable System) the entity clicked event is raised for parent entities recursivly
		// Default is true
		inline void SetBubbleUp(bool value = true) { bubbleUp = value; }

		// Sets whether the entity clicked event also gets raised for all the parent entities
		inline const bool IsClickAbsorebd() const { return absorbeClick; }

		inline const bool DoesBubbleUp() const { return bubbleUp; }

	private:
		// Is this necessarry?
		//void RemoveExpiredConnectedClickableEntities();

	private:
		// List of entities for which a mbe::EntityClickedEvent is raised when this entity is clicked
		// make sure that the list only goes one way - to avoid infinite loops
		// The set guarantees that there no entity id can be inserted multiple times.
		//std::set<Entity::HandleID> connectedClickableEntityIdList;

		bool absorbeClick;
		bool bubbleUp;
	};

} // namespace mbe