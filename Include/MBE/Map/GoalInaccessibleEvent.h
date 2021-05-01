#pragma once

/// @file
/// @brief Class mbe::GoalInaccessibleEvent

#include <MBE/Core/Entity.h>

namespace mbe
{
	namespace event
	{

		class GoalInaccessibleEvent
		{
		public:
			GoalInaccessibleEvent();
			GoalInaccessibleEvent(Entity::ID entityId);
			~GoalInaccessibleEvent() = default;

		public:
			inline Entity::ID GetEntityID() const { return entityId; }

			inline void SetEntityID(Entity::ID id) { entityId = id; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream& operator << (std::ostream& stream, const GoalInaccessibleEvent& event);

		private:
			Entity::ID entityId;
		};

	} // namespace event
} // namespace mbe