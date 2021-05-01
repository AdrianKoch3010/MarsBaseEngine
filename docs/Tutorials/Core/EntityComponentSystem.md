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

The technique of composition outlined above would allow for dynamic creation and removal of components. I have decided against this as it introduces a whole bunch of new problems.

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
        std::forward_as_tuple("Tree"),
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

### Example

Let's suppose we want to create a character. We might want it to have a position on the map, an inventory, some animations and a sprite. It should be able to play an animation or sound effect that moves with the character should a certain event occur. All of these things can be broken up into seperate entities with their respective components. The above example might look something like this:

``` XML
<!-- Top-level entity -->
<Entity id="0" parentId="-1">
    <Component type="TransformComponent">
        <Position x="32" y="64"/>
        <Origin x="0" y="0"/>
        <Scale x="1" y="1"/>
        <Rotation>0</Rotation>
    </Component>
    <Component type="TileMapComponent">
        <Position x="1" y="2" />
        <MovementSpeedShape>
            <Row>10, 10, 10</Row>
            <Row>10, 10, 10</Row>
            <Row>10, 10, 10</Row>
        </MovementSpeedShape>
    </Component>
    <!-- Playing an animation on this entity will attempt to play the same animation on its child render entity -->
    <Component type="AnimationComponent" />
</Entity>
<!-- Render entity set as a child -->
<Entity id="1" parentId="0">
    <!-- The transform component will be relative to its parent -->
    <Component type="TransformComponent">
        <Position x="0" y="42"/>
        <Origin x="-11.11" y="0.5"/>
        <Scale x="1" y="1"/>
        <Rotation>45.9</Rotation>
    </Component>
    <Component type="RenderInformationComponent">
        <RenderLayer>Objects</RenderLayer>
        <ZOrder>1000000</ZOrder>
    </Component>
    <Component type="TextureWrapperComponent">
        <ActiveTexture>1</ActiveTexture>
        <Textures>
            <Texture><TextureWrapper>Texture1</TextureWrapper></Texture>
            <Texture><TextureWrapper>Texture2</TextureWrapper></Texture>
        </Textures>
    </Component>
    <Component type="SpriteRenderComponent">
        <Colour>
            <R>255</R>
            <G>255</G>
            <B>255</B>
            <A>255</A>
        </Colour>
    </Component>
    <Component type="PixelMaskClickableComponent">
        <AbsorbeClick>true</AbsorbeClick>
        <BubbleUp>true</BubbleUp>
    </Component>
    <Component type="AnimationComponent">
        <Animator id="walkanimator">
            <Progress>0.97901994</Progress>
            <Loop>true</Loop>
            <Paused>false</Paused>
            <Animations>
                <Animation type="FrameAnimation" id="walkdown" duration="1000">
                    <Frame>
                        <Duration>0.5</Duration>
                        <SubRect>
                            <Top>128</Top><Left>0</Left><Width>64</Width><Height>64</Height>
                        </SubRect>
                    </Frame>
                    <Frame>
                        <Duration>0.5</Duration>
                        <SubRect>
                            <Top>128</Top><Left>64</Left><Width>64</Width>Height>64</Height>
                        </SubRect>
                    </Frame>
                </Animation>
            </Animations>
        </Animator>
        <Animator id="blinkinganimator">
            <Progress>0</Progress>
            <Loop>false</Loop>
            <Paused>false</Paused>
            <Animations>
                <Animation type="BlinkingAnimation" id="blink" duration="2000">
                    <MinimumBrightness>0.25</MinimumBrightness>
                </Animation>
            </Animations>
        </Animator>
    </Component>
</Entity>
```

Note how this is the same XML format used by the [entity serialiser](Serialisation.md). Of course the same outcome can be achived by creating the entity and code and adding the required components.