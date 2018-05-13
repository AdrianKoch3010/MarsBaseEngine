#pragma once

/// @file
/// @brief Class mbe::InputHandler

#include <map>
#include <vector>
#include <algorithm>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/Utility.h>
#include <MBE/Input/MouseButtonEvents.h>
#include <MBE/Input/ActionEvents.h>

using mbe::event::MouseButtonPressedEvent;
using mbe::event::MouseButtonHoldEvent;
using mbe::event::MouseButtonReleasedEvent;

using mbe::event::ActionBaseEvent;
using mbe::event::ActionPressedEvent;
using mbe::event::ActionHoldEvent;
using mbe::event::ActionReleasedEvent;

namespace mbe
{
	/// @brief Handels mouse and keyboard events as well as realtime input and raises respective events
	/// @details Furthermore, the input handler allows for subscribing keys to actions which allows for dynamic key bindings.
	/// @note The action strings are not case sensitive so capital letters don't matter. <b>Use ACII characters only!<b>
	/// When an action string or a list of action strings is returned, they are always normalised. To compare an action string,
	/// call mbe::NormaliseIDString() on it before the comparision.
	class InputHandler
	{
	public:
		/// @brief Default constructor
		InputHandler() = default;

		/// @brief Default destructor
		~InputHandler() = default;

		/// @brief Raises respective events for mouse button events and any registered key released events
		/// @details This function should be called within the poll event loop of the window.
		/// It checks for mouse clicks and any registred key release events and raises the according
		/// events when they occur. Generally speaking the handleEvent method checks for changes in state.
		/// To check whether a key or mouse button is currently pressed use the mbe::InputHandler::HandleRealtimeInput() method.
		/// @param event A reference to the event that is currently polled in the poll event loop of the window.
		/// @param eventManager The EventManager which is used to raise the events
		void HandleEvent(const sf::Event & event, EventManager & eventManager);

		/// @brief Raises respective events for the mouse buttons or any registered keys that are currently pressed
		/// @details This function should not be called within the poll event loop of the window.
		/// Generally speaking, this function checks the current state. To check for changes in state (i.e. sf::Event)
		/// use the mbe::InputHandler::HandleEvent() method.
		/// @param eventManager The mbe::EventManager which is used to raise the events
		/// @param window The sf::Window to which the mouse position is relative to
		void HandleRealtimeInput(EventManager & eventManager, sf::Window & window);

		/// @brief Assigns a mouse button to an action
		/// @details More than one button can be assigned to the same action.
		/// A button cannot be assigned to multiple action. If done so the old assigned action binding
		/// will be overwritten.
		/// @param action The action that will be passed as data of one of the action events
		/// @param button The mouse button to be assigned
		/// @param isRealtimeInput If set to true, it is checked whether the button is pressed and
		/// events are raised in the HandleRealtimeInput method. If set to false, it is checked whether
		/// a mouse button released event has occured and events are raised in the HandleEvent() method.
		void AssignMouseButton(ActionBaseEvent::Action action, sf::Mouse::Button button, bool isRealTimeInput = false);

		/// @brief Returns a list of the mouse buttons assigned to the past in action
		/// @param action The action for which the buttons will be fetched
		/// @returns A list of buttons. If no buttons are assigned, an empty list will be returned
		std::vector<sf::Mouse::Button> GetAssignedMouseButtons(ActionBaseEvent::Action action) const;

		/// @brief Assigns a key to an action
		/// @details More than one key can be assigned to the same action.
		/// A key cannot be assigned to multiple action. If done so the old assigned action binding
		/// will be overwritten.
		/// @param action The action that will be passed as data of one of the action events
		/// @param key The key to be assigned
		/// @param isRealtimeInput If set to true, it is checked whether the key is pressed and
		/// events are raised in the HandleRealtimeInput method. If set to false, it is checked whether
		/// a key released event has occured and events are raised in the HandleEvent() method.
		void AssignKey(ActionBaseEvent::Action action, sf::Keyboard::Key key, bool isRealtimeInput = false);

		/// @brief Returns a list of the keys assigned to the past in action
		/// @param action The action for which the keys will be fetched
		/// @returns A list of keys. If no keys are assigned, an empty list will be returned
		std::vector<sf::Keyboard::Key> GetAssignedKeys(ActionBaseEvent::Action action) const;

		/// @brief Returns whether an action is active
		/// @details This method allows for checking directly whether a specific action is
		/// currently active. An action is active when one of the assigned mouse buttons or keys
		/// are pressed (sf::Mouse::isButtonPressed() or sf::Keyboard::isKeyPressed() returns true).
		/// If an action is active the respective mbe::event::ActionHoldEvent will also be raised.
		/// @param action The action that is checked for whether it is active / hold
		/// @returns True if the action is active / hold, flase otherwise
		bool IsActionActive(ActionBaseEvent::Action action) const;

	private:
		std::map<sf::Keyboard::Key, ActionBaseEvent::Action> eventKeyBindings;
		std::map<sf::Keyboard::Key, ActionBaseEvent::Action> realtimeInputKeyBindings;

		std::map<sf::Mouse::Button, ActionBaseEvent::Action> eventMouseButtonBindings;
		std::map<sf::Mouse::Button, ActionBaseEvent::Action> realtimeMouseButtonBindings;
	};

} // namespace mbe