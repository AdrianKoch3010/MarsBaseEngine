# Handle IDs

The HandleBase and HandleID classes provide an easy way to get a unique identifier for an object as well as way of knowing whether the object accessed through the handle id is still valid.

## HandleBase

- Inherit from ```HandleBase<TDerived>```

- The object can be accessed through the id
The cachedPointer will make this access fast
Valid check only when compiled with the _DEBUG FLAG enabled

### Example

## ID and ConstID types
The constness of the id is independent of that of the object refered to by the id.


### Examaples
```c++
auto& entity = entityManager.CreateEntity();
const auto& constEntity = entity;
// Entity::ID id = constEntity.GetHandleID(); no ok (Entity would no longer be const))
Entity::ConstID cid0 = constEntity.GetHandleID(); // ok
Entity::ID id0 = entity.GetHandleID(); // ok

Entity::ConstID cid1(id0);
Entity::ConstID cid2 = id0;

```


## Entity::ID
Instead of just checking whether the object has been deleted, the ```Entity::ID::Valid()``` method also checks whether the entity is active or not and only returns true if the entity is both alive and active.

This has the advantage that entities which are marked to be deleted (by calling ```entity.Delete()```) and thus no longer active will act as if they have been deleted when accessed through their handle id.