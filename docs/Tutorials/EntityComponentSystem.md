# Entity Component System

The entity component system consists of three main parts.

At the core of the engine lies the entity component system. Contrary to an object oriented approach, state and behaivour are sperated into components and systems respectivly. Entities act as containers for a collection of components that make up the entity. The underlying pardigm of this approach is to use **composition** rather than **inheritance**; in other words, the components deterimne the type of an entity and, therefore, what kind of game object it is.

## Comparison to Object Oriented Approach

### Object Oriented design

In a classic OO design all game objects inherit from a central


## Dynamic Entity Types

The technique of composition outlined above would allow for dynamic creation and removal of entities. I have decided against this as it introduces a whole bunch of new problems.

<!-- Give example of problem -->

Therefore, all components should be added when an entity is created. After all components have been added, an entity created event must be raised to let the other systems know that a new entity has been created. These systems will then assume that the components that make up the entity don't change.

> Example

```c++
// Create the entity
auto & entity = entityManager.CreateEntity();

// Add the components
entity.AddComponent<mbe::TransformComponent>();
entity.AddComponent<mbe::TileMapcomponent>();
entity.AddComponent<mbe::AnimationComponent>();

// Tell the other systems that a new entity has been created
eventManager.RaiseEvent(mbe::event::EntityCreatedEvent(entity.GetHandleID()));
```

## An alternative - Child Entities

In order to compensate for the loss in flexibility caused by the lack of dynamic entity types, entities can be organised in a tree structure. Making an entity a child of another can have multiple effects.

- The life time is coupled - if the parent is destroyed so are its children
- Some systems make use of this structure
    - Animation propagates to the child entities
    - Events such as the entity clicked event bubble up the parents
- The child can be accessed through its parent and vice versa

This provides a neat way to compose a game obejct, dynamically add and remove state and behavour as well as organise the game world in a scene graph mannor.

> Example

Let's suppose we want to create a character. We might want it to have a position on the map, an inventory, some animations and a sprite. It should be able to put on a hat or play a sound effect that moves with the character should a certain event occur.