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

	for (auto transformEntityId = this->parentEntity.GetHandleID(); transformEntityId.Valid() && transformEntityId->HasComponent<TransformComponent>(); )
	{
		// This should be guaranteed by the SetRelativeTo(parentTransformEntityId) method / constructor
		assert(transformEntityId->HasComponent<TransformComponent>() && "TransformComponent: The entity must have a mbe::TransformComponent");

		// transform = transformComponent->getTransform() * transform;
		transform = transformEntityId->GetComponent<TransformComponent>().GetLocalTransform() * transform;

		// transformComponent = transformComponent->parent
		transformEntityId = transformEntityId->GetParentEntityID();
	}

	return transform;
}
