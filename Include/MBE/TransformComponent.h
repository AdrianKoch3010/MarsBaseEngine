#pragma once

#include <SFML/Graphics/Transformable.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{
	// wrapper around sf::Transformable
	class TransformComponent : public Component
	{
	public:
		TransformComponent(Entity & parentEntity, Entity::HandleID parentTransformEntityId = Entity::GetNullID());
		~TransformComponent() = default;

	public:
		inline void Update(sf::Time frameTime) override {}

		// Makes this entity relative to the transform component of another entity
		// Passing in mbe::Entity::GetNullID() sets this makes this entity absolute again so calling
		// this->GetWorldTransform() will return the same as this->GetTransform()
		void SetRelativeTo(Entity::HandleID parentTransformEntityId);

		inline void SetPosition(const sf::Vector2f & position) { transformable.setPosition(position); }

		inline void SetRotation(float angle) { transformable.setRotation(angle); }

		inline void SetScale(const sf::Vector2f & factors) { transformable.setScale(factors); }

		inline void SetOrigin(const sf::Vector2f origin) { transformable.setOrigin(origin); }

		inline const sf::Vector2f & GetPosition() const { return transformable.getPosition(); }

		inline float GetRotation() const { return transformable.getRotation(); }

		inline const sf::Vector2f & GetScale() const { return transformable.getScale(); }

		inline const sf::Vector2f & GetOrigin() const { return transformable.getOrigin(); }

		inline void Move(const sf::Vector2f & offset) { transformable.move(offset); }

		inline void Rotate(float angle) { transformable.rotate(angle); }

		inline void Scale(const sf::Vector2f & factor) { transformable.scale(factor); }

		inline const sf::Transformable & GetTransformable() const { return transformable; }

		inline const sf::Transform & GetLocalTransform() const { return transformable.getTransform(); }

		inline const sf::Transform & GetInverseTransform() const { return transformable.getInverseTransform(); }

		// no const & since the value is calculates within the function
		sf::Vector2f GetWorldPosition() const;

		// no const & since the value is calculates within the function
		// Accumulates all the relative transforms until reaching the root i.e. an entity whose parent transform entiy is null
		// Returns the absolute transform of this entity
		sf::Transform GetWorldTransform() const;

	private:
		sf::Transformable transformable;
		Entity::HandleID parentTransformEntityId;
	};
} // namespace mbe