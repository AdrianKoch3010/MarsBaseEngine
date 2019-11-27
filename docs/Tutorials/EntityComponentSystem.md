# Entity Component System

The entity component system consists of three main parts.

At the core of the engine lies the entity component system. Contrary to an object oriented approach, state and behaivour are sperated into components and systems respectivly. Entities act as containers for a collection of components that make up the entity. The underlying pardigm of this approach is to use **composition** rather than **inheritance**; in other words, the components deterimne the type of an entity and, therefore, what kind of game object it is.

## Comparison to Object Oriented Approach

### Object Oriented Design

<!-- Maybe find a better example that illustrates the problem -->

In a classic OOP design all game objects inherit from a common super class. From there on, a sort of tree structure is used to progressivly add more behaviour and data to the derived components. For example, there might be two classes StaticGameObject and DynamicGameObject that inherit from GameObject. A Building might then inherit from StaticGameObject which might further have a StorageBarn inherit from it wheras a Character is likely to inherit from DynamicGameObject.

There are manifold problems related with this design. The main one comes down to flexibility. Let's say we want to create a movable storage. This should clearly inherit from StorageBarn as it will have the same data and functionality but should also be a DynamicGameObject. In C++ it is possible to do so. However, it is very error prone and the programmer is likely to end up with a diamond of death problem.

### ECS Design

In contrast, in an entity component system the entities (or game objects) are comprised of a number of components. Relating to the above example, there could be a MovementComponent, BuildingComponent and StorageComponent that can be combined in any way. The behaviour will then be determined by the systems acting on those entities and their components. This has another advantage; since systems are essentially independent functions (that may be implemented as classes) that should only communicate with other systems via data i.e. the components, they can be added or removed (enabled or disabled) at will without breaking other parts of the code. This is something very hard to realise in the OOP approach.


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

There is another way which allows adding multiple components as once. 
```c++
auto & tree = entityManager.CreateEntity();
tree3.AddComponents<
    mbe::TransformComponent,
    mbe::RenderInformationComponent,
    mbe::TextureWrapperComponent,
    mbe::SpriteRenderComponent,
    mbe::TopDownInformationComponent>(
        std::forward_as_tuple(),
        std::forward_as_tuple(RenderLayer::Objects),
        std::forward_as_tuple(context.textureWrappers["Tree"]),
        std::forward_as_tuple(),
        std::forward_as_tuple()
        );
eventManager.RaiseEvent(EntityCreatedEvent(tree.GetHandleID()));
tree.GetComponent<mbe::TransformComponent>().SetPosition({ 300.f, 100.f });
```

## Parent and Child Entities

In order to compensate for the loss in flexibility caused by the lack of dynamic entity types, entities can be organised in a tree structure. Making an entity a child of another can have multiple effects.

- The life time is coupled - if the parent is destroyed so are its children
- Some systems make use of this structure
    - Animation propagates to the child entities
    - Events such as the entity clicked event bubble up the parents
- The child can be accessed through its parent and vice versa

This provides a neat way to compose a game obejct, dynamically add and remove state and behavour as well as organise the game world in a scene graph mannor.

<!-- Ellaborate on the example -->

> Example

Let's suppose we want to create a character. We might want it to have a position on the map, an inventory, some animations and a sprite. It should be able to put on a hat or play a sound effect that moves with the character should a certain event occur.