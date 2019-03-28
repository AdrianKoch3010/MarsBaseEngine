# Documentation
The tutorial below provides some worked examples of how to use the Mars Base Engine.

>The full documentation can be found [here](Doxygen/index.html).

# Tutorial
## Table of Contents
- The entity component system
- Animation

### Entity Component System
#### Working with entities

| Entity		| Contains a list of components 	|
|:--------------|:----------------------------------|
| EntityManager	|								 	|
| Component		|									|

```c++
// The CreateEntity() method returns a reference, the actual object lives within the entity manager
auto & entity = entityManager.CreateEntity();
auto component = entity.AddComponent<MyComponent>();
````