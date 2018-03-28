#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <MBE/Input/ActionBaseEvent.h>

namespace mbe
{
	// Forward declare the mbe::InputHandler
	class InputHandler;

	namespace event
	{
		class ActionHoldEvent
		{
		public:
			typedef ActionBaseEvent::Action Action;

		public:
			explicit ActionHoldEvent(const InputHandler & inputHandler);
			~ActionHoldEvent() = default;

		public:
			// Throws an exception if the same action is added more than once
			void AddActiveAction(Action action);
			bool IsActionActive(Action action) const;

			inline const std::vector<Action> GetActiveActionList() const { return activeActionList; }

			inline const InputHandler & GetInputHandler() const { return inputHandler; }

			friend std::ostream & operator << (std::ostream & stream, const ActionHoldEvent & event);

		private:
			std::vector<Action> activeActionList;

			const InputHandler & inputHandler;
		};

	} // namespace event
} // namespace mbe
