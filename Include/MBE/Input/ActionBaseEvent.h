#pragma once

/// @file
/// @brief Class mbe::event::ActionBaseEvent

#include <string>

namespace mbe
{
	// Forward declare the mbe::InputHandler
	class InputHandler;

	namespace event
	{
		/// @brief Base class for the action pressed and action released events
		class ActionBaseEvent
		{
		public:
			typedef std::string Action;

		public:
			explicit ActionBaseEvent(const InputHandler & inputHandler, Action action = "");
			virtual ~ActionBaseEvent() = default;

			inline void SetAction(Action action) { this->action = action; }
			inline Action GetAction() const { return action; }

			inline const InputHandler & GetInputHandler() const { return inputHandler; }

		private:
			Action action;
			const InputHandler & inputHandler;
		};
	} // namespace event
} // namespace mbe