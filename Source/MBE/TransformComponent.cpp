#include <MBE/TransformComponent.h>

using namespace mbe;

TransformComponent::TransformComponent(EventManager & eventManager, Entity & parentEntity, Entity::HandleID parentTransformEntityId) :
	Component(eventManager, parentEntity)
{
	this->SetRelativeTo(parentTransformEntityId);
}

void TransformComponent::SetRelativeTo(Entity::HandleID parentTransformEntityId)
{
	// Make sure that the parent transform is different from this entity to avoid infinite loops when getting the world transform
	assert(parentTransformEntityId != parentEntity.GetHandleID() && "TransformComponent: This entity transform cannot be relative to itself");

	const auto * entityPtr = Entity::GetObjectFromID(parentTransformEntityId);

	// If the entity is not null (Entity::GetNullID()), make sure that it has a transform component
	if (entityPtr != nullptr)
		assert(entityPtr->HasComponent<TransformComponent>() && "TransformComponent: The entity must have a mbe::TransformComponent");

	// Assign the parent transform entity
	this->parentTransformEntityId = parentTransformEntityId;
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	// Transforms the origin using the absolute transform
	// equivalent to this->GetWorldTransform() * sf::Vector2f();
	return this->GetWorldTransform().transformPoint({ 0, 0 });
}

sf::Transform TransformComponent::GetWorldTransform() const
{
	// Recursivly transform combine the transforms of all parent entities

	sf::Transform transform{ sf::Transform::Identity };

	for (auto transformEntityId = this->parentEntity.GetHandleID(); Entity::GetObjectFromID(transformEntityId) != nullptr; )
	{
		// The entity's existance has been tested in the for loop
		const auto & entity = *Entity::GetObjectFromID(transformEntityId);

		// This should be guaranteed by the SetRelativeTo(parentTransformEntityId) method / constructor
		assert(entity.HasComponent<TransformComponent>() && "TransformComponent: The entity must have a mbe::TransformComponent");

		// transform = transformComponent->getTransform() * transform;
		transform = entity.GetComponent<TransformComponent>().GetLocalTransform() * transform;

		// transformComponent = transformComponent->parent
		transformEntityId = entity.GetComponent<TransformComponent>().parentTransformEntityId;
	}

	return transform;
}
