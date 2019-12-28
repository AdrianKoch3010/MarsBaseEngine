#include <MBE/TransformComponent.h>

using namespace mbe;

TransformComponent::TransformComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{
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

	for (auto transformEntityId = this->parentEntity.GetHandleID(); Entity::GetObjectFromID(transformEntityId) != nullptr && Entity::GetObjectFromID(transformEntityId)->HasComponent<TransformComponent>(); )
	{
		// The entity's existance has been tested in the for loop
		const auto & entity = *Entity::GetObjectFromID(transformEntityId);

		// This should be guaranteed by the SetRelativeTo(parentTransformEntityId) method / constructor
		assert(entity.HasComponent<TransformComponent>() && "TransformComponent: The entity must have a mbe::TransformComponent");

		// transform = transformComponent->getTransform() * transform;
		transform = entity.GetComponent<TransformComponent>().GetLocalTransform() * transform;

		// transformComponent = transformComponent->parent
		transformEntityId = entity.GetParentEntityID();
	}

	return transform;
}
