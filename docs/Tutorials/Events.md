# Events
Events can be used to communicate between systems.


The event manager is at the core of this system.

The type of an event is its id. Therefore, an event can be directly passed as an argument of the raise event method.
```c++
// This is unnecessary
eventManager.RaiseEvent<EventType>(event);
// Simply use
eventManager.RaiseEvent(event);
```

## Raising Events


## Subscribing to Events

## Important events

### Entity Created Event

Once all components have been added to the entity, an entity created event must be raised
```c++
mbe::event::EntityCreatedEvent event;
event.SetEntityID(entity.GetHandleID());
eventManager.RaiseEvent(event);

// This can be abbreviated to
eventManager.RaiseEvent(mbe::event::EntityCreatedEvent(entity.GetHandleID()));
```
