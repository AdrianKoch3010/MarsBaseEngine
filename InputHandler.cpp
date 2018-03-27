#include "InputHandler.h"

using namespace mbe;


void InputHandler::HandleEvent(const sf::Event & eve, EventManager & eventManager)
{
	if (eve.type == sf::Event::MouseButtonPressed)
	{
		// Raise the mouse button pressed event with the click position and clicked button as data
		MouseButtonPressedEvent mouseButtonPressedEvent(*this);
		mouseButtonPressedEvent.SetClickPosition(eve.mouseButton.x, eve.mouseButton.y);
		mouseButtonPressedEvent.SetMouseButton(eve.mouseButton.button);
		eventManager.RaiseEvent(mouseButtonPressedEvent);

		// Raise the according action pressed event
		auto it = eventMouseButtonBindings.find(eve.mouseButton.button);
		if (it != eventMouseButtonBindings.end())
			eventManager.RaiseEvent(ActionPressedEvent(*this, it->second));
	}
	else if (eve.type == sf::Event::MouseButtonReleased)
	{
		// Raise the mouse button released event with the click position and the clicked button as data
		MouseButtonReleasedEvent mouseButtonReleasedEvent(*this);
		mouseButtonReleasedEvent.SetClickPosition(eve.mouseButton.x, eve.mouseButton.y);
		mouseButtonReleasedEvent.SetMouseButton(eve.mouseButton.button);
		eventManager.RaiseEvent(mouseButtonReleasedEvent);

		// Raise according action released event
		auto it = eventMouseButtonBindings.find(eve.mouseButton.button);
		if (it != eventMouseButtonBindings.end())
			eventManager.RaiseEvent(ActionReleasedEvent(*this, it->second));
	}
	else if (eve.type == sf::Event::KeyPressed)
	{
		// Raise the accoridng action pressed event
		auto it = eventKeyBindings.find(eve.key.code);
		if (it != eventKeyBindings.end())
			eventManager.RaiseEvent(ActionPressedEvent(*this, it->second));
	}
	else if (eve.type == sf::Event::KeyReleased)
	{
		// Raise the accorfing action released event
		auto it = eventKeyBindings.find(eve.key.code);
		if (it != eventKeyBindings.end())
			eventManager.RaiseEvent(ActionReleasedEvent(*this, it->second));
	}
}

void InputHandler::HandleRealtimeInput(EventManager & eventManager, sf::Window & window)
{
	// Raise mouse button hold event
	MouseButtonHoldEvent mouseButtonHoldEvent(*this);
	bool mouseButtonHold = false;

	// Add the buttons pressed
	// Note that more than one button can be pressed at a time
	for (size_t button = 0u; button < sf::Mouse::ButtonCount; button++)
	{
		if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button)))
		{
			mouseButtonHoldEvent.AddPressedButton(static_cast<sf::Mouse::Button>(button));
			mouseButtonHold = true;
		}
	}

	if (mouseButtonHold)
	{
		mouseButtonHoldEvent.SetClickPosition(sf::Mouse::getPosition(window));
		eventManager.RaiseEvent(mouseButtonHoldEvent);
	}

	// Raise action hold event for key and mouse bindings
	ActionHoldEvent actionHoldEvent(*this);
	bool actionActive = false;

	// Add active mouse button action
	for (size_t itButton = 0u; itButton < sf::Mouse::ButtonCount; itButton++)
	{
		auto button = static_cast<sf::Mouse::Button>(itButton);
		if (sf::Mouse::isButtonPressed(button))
		{
			auto it = realtimeMouseButtonBindings.find(button);
			if (it != realtimeMouseButtonBindings.end())
			{
				actionHoldEvent.AddActiveAction(it->second);
				actionActive = true;
			}
		}
	}
	// Add active key actions
	for (size_t itKey = 0u; itKey < sf::Keyboard::KeyCount; itKey++)
	{
		auto key = static_cast<sf::Keyboard::Key>(itKey);
		if (sf::Keyboard::isKeyPressed(key))
		{
			auto it = realtimeInputKeyBindings.find(key);
			if (it != realtimeInputKeyBindings.end())
			{
				actionHoldEvent.AddActiveAction(it->second);
				actionActive = true;
			}
		}
	}
	// Raise the action hold event if at least one action is active
	if (actionActive)
		eventManager.RaiseEvent(actionHoldEvent);
}

void mbe::InputHandler::AssignMouseButton(ActionBaseEvent::Action action, sf::Mouse::Button button, bool isRealTimeInput)
{
	if (isRealTimeInput)
		realtimeMouseButtonBindings[button] = action;
	else
		eventMouseButtonBindings[button] = action;
}

std::vector<sf::Mouse::Button> mbe::InputHandler::GetAssignedMouseButtons(ActionBaseEvent::Action action) const
{
	// Create an empty mouse button list
	std::vector<sf::Mouse::Button> mouseButtonList;

	// Loop through the event key bindings
	for (const auto pair : eventMouseButtonBindings)
	{
		if (pair.second == action)
			mouseButtonList.push_back(pair.first);
	}

	// Loop through the realtime input key bindings
	for (const auto pair : realtimeMouseButtonBindings)
	{
		if (pair.second == action)
			mouseButtonList.push_back(pair.first);
	}

	return mouseButtonList;
}

void InputHandler::AssignKey(ActionBaseEvent::Action action, sf::Keyboard::Key key, bool isRealtimeInput)
{
	if (isRealtimeInput)
		realtimeInputKeyBindings[key] = action;
	else
		eventKeyBindings[key] = action;
}

std::vector<sf::Keyboard::Key> InputHandler::GetAssignedKeys(ActionBaseEvent::Action action) const
{
	// Create an empty key list
	std::vector<sf::Keyboard::Key> keyList;

	// Loop through the event key bindings
	for (const auto pair : eventKeyBindings)
	{
		if (pair.second == action)
			keyList.push_back(pair.first);
	}

	// Loop through the realtime input key bindings
	for (const auto pair : realtimeInputKeyBindings)
	{
		if (pair.second == action)
			keyList.push_back(pair.first);
	}

	return keyList;
}

bool InputHandler::IsActionActive(ActionBaseEvent::Action action) const
{
	// Get the associated mouse buttons
	auto assignedMouseButtons = GetAssignedMouseButtons(action);

	// If one of the associated mouse buttons is pressed, the action is active
	for (const auto & mouseButton : assignedMouseButtons)
	{
		if (sf::Mouse::isButtonPressed(mouseButton))
			return true;
	}


	// Get the associated keys
	auto assignedKeys = GetAssignedKeys(action);

	// If any one of the associated keys is pressed, the action is active
	for (const auto & key : assignedKeys)
	{
		if (sf::Keyboard::isKeyPressed(key))
			return true;
	}
	return false;
}
