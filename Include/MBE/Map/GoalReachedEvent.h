#pragma once

/// @file
/// @brief Class mbe::GoalReachedEvent

#include <MBE/Core/Entity.h>

namespace mbe
{
	namespace event
	{

		class GoalReachedEvent
		{
		public:
			GoalReachedEvent();
			GoalReachedEvent(Entity::HandleID entityId);
			~GoalReachedEvent() = default;

		public:
			inline Entity::HandleID GetEntityID() const { return entityId; }

			inline void SetEntityID(Entity::HandleID id) { entityId = id; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream & operator << (std::ostream & stream, const GoalReachedEvent & event);

		private:
			Entity::HandleID entityId;
		};

	} // namespace event
} // namespace mbe