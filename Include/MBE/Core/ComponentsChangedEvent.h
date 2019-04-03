#pragma once

/// @file
/// @brief Class mbe::event::ComponentsChangedEvent

#include <iostream>

namespace mbe
{
	//Forward declare mbe::Entity
	class Entity;

	namespace event
	{
		/// @brief Contains the id of the mbe::Entity whose components composition has changed
		/// @details This event is raised automatically by the entity give systems a chance to react
		class ComponentsChangedEvent
		{
		public:
			/// @brief Constructor
			/// @param entityId The entity whose components composition has changed
			ComponentsChangedEvent(Entity & entity);

			/// @brief Default destructor
			~ComponentsChangedEvent() = default;

		public:
			/// @brief Returns the id of the mbe::Entity whose components composition has changed
			inline Entity & GetEntity() const { return entity; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream & operator << (std::ostream & stream, const ComponentsChangedEvent & event);

		private:
			Entity & entity;
		};
	} // namespace event
} // namespace mbe