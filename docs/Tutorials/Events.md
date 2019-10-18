# The Event System
Events can be used to communicate between systems. Generally speaking, events should be used to communicate that something has happened rather than say that something should happen. Examples would be 'MouseClickedEvent' or 'GoalReachedEvent' rather than 'MoveCharacterEvent'. This is to avoid invisible dependencies between systems.


The event manager is at the core of this system. Every object / system that wants to raise or subscribe to events needs a reference to it. Usually, the event manager will be created with the game state and only be deleted when the game (or game state) is. This means that it is save to pass references to it. 


## Raising Events

The **type** of an event is its id. Therefore, an event can be directly passed as an argument of the raise event method.

```c++
// This is unnecessary
eventManager.RaiseEvent<EventType>(event);
// Simply use
eventManager.RaiseEvent(event);
```

## Subscribing to Events

The event manager allows to subscribe functions to a specific event that will be called when that event is raised. The function must take an event object of that type as input. The subscribe method returns a subscription id that can be used to desubscribe from the event. This will ususally be done in the destructor of the class that subscribed to that event. The TCallback template is just so that the function signature must not be remembered.

```c++
// Here we subscribe to the mouse button released event
// This function will be called every time the user releases a mouse button
mouseButtonReleasedSubscription = eventManager.Subscribe(mbe::EventManager::TCallback<MouseButtonReleasedEvent>([/*capture*/](const MouseButtonReleasedEvent & event)
{
    if (event.GetButton() == sf::Mouse::Button::Left)
    {
        // Do something
    }
}
```

Don't forget to unsubsribe!

```c++
// In the destructor

// This workes
eventManager.UnSubscribe(mouseButtonReleasedSubscription);
// But this good practice and (much) more efficient
eventManager.UnSubscribe<MouseButtonReleasedEvent>(mouseButtonReleasedSubscription);
```

## Important events

### Entity Created Event

Once all components have been added to the entity, an entity created event must be raised. This is to notify other systems of the creation of the new entity. The entity is asumed to be complete at that point and no more components should be added from now on (although there might be exceptions to that rule)
```c++
mbe::event::EntityCreatedEvent event;
event.SetEntityID(entity.GetHandleID());
eventManager.RaiseEvent(event);

// This can be abbreviated to
eventManager.RaiseEvent(mbe::event::EntityCreatedEvent(entity.GetHandleID()));
```

### EntityClickedEvent

This event will be raised when an entity has been clicked given that it has a clcikable component. Depending on the bubbeling behaviour, it will be raised for the entity's parents as well. Unless specified otherwise, the click will pass through object (ie. it won't be absorbed by the object drawn on top) and it'll bubble up, meaning that there will be an EntityClickedEvent raised for all the parents recursively.

### Input Events
There is a range of events that are raised when user input occurs. These are

- ActionPressedEvent
- ActionReleasedEvent
- ActionHoldEvent
- MouseButtonPressedEvent
- MouseButtonReleasedEvent
- MouseButtonHoldEvent


For more information on how user input is handled have a look at the [InputHandler](InputHandler.md).