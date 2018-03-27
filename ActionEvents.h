#pragma once

#include <iostream>
#include <string>

#include "ActionBaseEvent.h"
#include "ActionHoldEvent.h"

using mbe::event::ActionBaseEvent;

namespace mbe
{
	namespace event
	{
		class ActionPressedEvent : public ActionBaseEvent
		{
		public:
			explicit inline ActionPressedEvent(const InputHandler & inputHandler, Action action = "") : ActionBaseEvent(inputHandler, action) {}
			friend std::ostream & operator << (std::ostream & stream, const ActionPressedEvent & event);
		};

		class ActionReleasedEvent : public ActionBaseEvent
		{
		public:
			explicit inline ActionReleasedEvent(const InputHandler & inputHandler, Action action = "") : ActionBaseEvent(inputHandler, action) {}
			friend std::ostream & operator << (std::ostream & stream, const ActionReleasedEvent & event);
		};

	} // namespace event
} // namespace mbe