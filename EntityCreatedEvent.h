#pragma once

/// @file
/// @brief Class mbe::event::EntityCreatedEvent

#include <iostream>
#include <string>
#include <memory>
#include <limits>

#include "Entity.h"

namespace mbe
{
	namespace event
	{
		/// @brief Contains the id of the mbe::Entity that has been created
		/// @details This event should be raised when an entity has been created and all components have been added
		/// @attention It is important to raise this event once all components have been added. Systems may be subscribed to this event
		/// and add this entity depending on its components. If not requried components have been added by the time this event is risen
	    /// the entity might not register in all the systems it is supposed to.
		class EntityCreatedEvent
		{
		public:
			/// @brief Default constructor
			/// @details When using this constructor the entity id is set to null making sure that no mbe::Entity exists under that id.
			/// @note When using the default constructor, the entity id should be set before raising the event.
			/// This can be done using the mbe::EntityCreatedEvent::SetEntityID() method.
			EntityCreatedEvent();

			/// @brief Constructor
			/// @param entityId The id of the entity that has been created
			EntityCreatedEvent(Entity::HandleID entityId);

			/// @brief Default destructor
			~EntityCreatedEvent() = default;

		public:
			/// @brief Sets the entity id of the mbe::Entity that has been created
			/// @details This method is only needed when calling the default constructor.
			/// @param id The id of the entity that has been created
			inline void SetEntityID(Entity::HandleID id) { entityId = id; }

			/// @brief Returns the id of the mbe::Entity that has been created
			inline Entity::HandleID GetEntityID() const { return entityId; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream & operator << (std::ostream & stream, const EntityCreatedEvent & event);

		private:
			Entity::HandleID entityId;
		};
	} // namespace event
} // namespace mbe