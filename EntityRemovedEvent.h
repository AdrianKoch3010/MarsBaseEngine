#pragma once

/// @file
/// @brief Class mbe::event::EntityRemovedEvent

#include <iostream>
#include <string>
#include <memory>
#include <limits>

#include "Entity.h"

namespace mbe
{
	namespace event
	{
		/// @brief Contains the id of the mbe::Entity that has been removed
		/// @details This event is only needed for removing a mbe::Entity from all systems
		/// that is not deleted immediately e.g. in order to add it again later.
		class EntityRemovedEvent
		{
		public:
			/// @brief Default constructor
			/// @details When using this constructor the entity id is set to null making sure
			/// that no mbe::Entity exists under that id.
			/// @note When using the default constructor, the entity id should be set before raising the event.
			/// This can be done using the mbe::EntityRemovedEvent::SetEntityID() method.
			EntityRemovedEvent();

			/// @brief Constructor
			/// @param entityId The id of the mbe::Entity that has been removed
			EntityRemovedEvent(Entity::HandleID entityId);

		public:
			/// @brief Default Destructor
			~EntityRemovedEvent() = default;

			/// @brief Sets the entity id of the mbe::Entity that has been removed
			/// @details This method is only needed when calling the default constructor.
			/// @param id The id of the mbe::Entity that has been removed
			inline void SetEntityID(Entity::HandleID id) { entityId = id; }

			/// @brief Returns the id of the mbe::Entity that has been removed
			/// @details Meant for internal use by systems such as the mbe::RenderSystem
			inline Entity::HandleID GetEntityID() const { return entityId; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream & operator << (std::ostream & stream, const EntityRemovedEvent & event);

		private:
			Entity::HandleID entityId;
		};
	} // namespace event
} // namespace mbe