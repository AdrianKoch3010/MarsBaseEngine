# Handling User Input

The input handler connects the SFML input events with the [event system](Events.md) used by the engine. 

## Input Types

The engine supports mouse and keyboard input as well as providing a level of abstraction that allows for configurable key/mouse bindings. This is achieved through the concept of actions that can map onto both keyboard and mouse input.

Further, the input handler provides two ways of dealing with user input: events and real-time input. Events are raised by the event manager whereas real-time input can be queried directly using the ```IsActionActive()``` method. 

### Using SFML types

The engine uses the same key (and mouse) types as SFML which makes interfacing easier and avoids an unecessary duplication of code. Documentation can be found [here](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Mouse.php) (sf::Mouse) and [here](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Keyboard.php) (sf::Keyboard).

## Input Events

There is a range of events that are raised when user input occurs. These are

- ActionPressedEvent
- ActionReleasedEvent
- ActionHoldEvent
- MouseButtonPressedEvent
- MouseButtonReleasedEvent
- MouseButtonHoldEvent

Note that any of the action events may have been caused by a mouse button been pressed, hold or released (as well as keyboard events). The distincion between this and 

## Key Bindings

A big part of a good input system is to allow the user to assign the keys themselves. This can be achieved by introducing a layer of abstraction - decoupling the actual physical input from the corresponding logical action.

Keys and mouse button are assigned seperately. They both map onto the same set of actions, however. This means that the same action may be invoked by both keyboard and mouse input. In the example below pressing A or the left mouse button will raise events for the same Action

```c++
bool isRealtimeInput = true;
inputHandler.AssignKey("Action", sf::Keyboard::Key::A, isRealtimeInput);
inputHandler.AssignMouseButton("Action", sf::Mouse::Left); // isRealtimeInput = false by default
```

A list of the keys and mouse buttons assigned to a specific action can be obtained using the following two mehthods

```c++
auto mouseButtons = inputHandler.GetAssignedMouseButtons("Action");
auto keys = inputHandler.GetAssignedKeys("Action");
```

Lastly, actions can be queried directly as described earlier. This is independed of whether they have been registered as realtime input or not.

```c++
bool active = inputHandler.IsActionActive("Action");
```

>Example

Lets suppose we want to place some building when the user holds down the key that has been assigned to the building action and then releases the left mouse button. This will require us to subscribe to the ```mbe::event::MouseButtonReleasedEvent``` and implement the functionality in subscribed function. If you are not sure how the event system works have a look [here](Events.md).

```c++
using mbe::event::MouseButtonReleasedEvent;

// Assign the B key to the build action. This isn't a realtime action (false by default)
inputHandler.AssignKey("Build", sf::Keyboard::Key::B);

mouseButtonReleasedSubscription = eventManager.Subscribe(mbe::EventManager::TCallback<MouseButtonReleasedEvent>([&, this](const MouseButtonReleasedEvent & event)
{
	if (event.GetButton() != sf::Mouse::Button::Left)
    	return;
	
    // There is no need to use the sf::Vector2f type, just for convenience as it already exists
	sf::Vector2f clickPosition = { (float)event.GetClickPosition().x, (float)event.GetClickPosition().y };
    // We can obtain a reference to the input handler that raised the event
	const auto & inputHandler = event.GetInputHandler();
	
	if (inputHandler.IsActionActive("Build"))
	{
		// Perform building code possibly using the map to find the corresponding map position
	}
}
```