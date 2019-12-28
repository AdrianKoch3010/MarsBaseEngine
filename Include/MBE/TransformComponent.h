#pragma once

/// @file
/// @brief class mbe::TransformComponent

#include <SFML/Graphics/Transformable.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{

	/// @brief Stores a transformation matrix and provides a convenient interaface
	/// @details It is esentially a wrapper around the sf::Transformable class. The underlying transformable can be retrieved using
	/// the GetTransformable() methdod.
	/// @n The transform is relative to the parent entity. To get the actual position and transform, use the GetWorldPosition() and GetWorldTransform() methods.
	class TransformComponent : public Component
	{
	public:
		/// @brief Constructor
		/// @param eventManager A reference to the mbe::EventManager
		/// @param parentEntity A reference to the mbe::Entity this component belongs to
		TransformComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief Default destructor
		~TransformComponent() = default;

	public:
		/// @brief Set the (relative) position
		/// @details This function completely overwrites the previous position.
		/// See the Move() method to apply an offset based on the previous position instead.
		/// The default position of a transformable object is { 0.f, 0.f }.
		/// @param position The new position
		/// @see Move, GetPosition
		inline void SetPosition(const sf::Vector2f & position) { transformable.setPosition(position); }

		/// @brief Set the (relative) rotation
		/// @details This function completely overwrites the previous rotation.
		/// See the Rotate() method to add an angle based on the previous rotation instead.
		/// The default rotation of a transformable object is 0.
		/// @param angle The new angle of rotation in degrees
		/// @see Rotate, GetRotation
		inline void SetRotation(float angle) { transformable.setRotation(angle); }

		/// @brief Set the (relative) scale factors
		/// @details This function completely overwrites the previous scale value.
		/// See the Scale() method to add a factor based on the previous scale instead.
		/// The default scale of a transformable object is { 1.f, 1.f }.
		/// @param factors New scale factors
		/// @see Scale, GetScale
		inline void SetScale(const sf::Vector2f & factors) { transformable.setScale(factors); }

		/// @brief set the local origin of the object
		/// @details The origin of an object defines the center point for
		/// all transformations (position, scale, rotation).
		/// The coordinates of this point must be relative to the top-left corner of the object, and ignore all
		/// transformations (position, scale, rotation). The default origin of a transformable object is { 0.f , 0.f }.
		/// @param origin New origin
		/// @see GetOrigin
		inline void SetOrigin(const sf::Vector2f origin) { transformable.setOrigin(origin); }

		/// @brief Get the (relative) position
		/// @returns Current position
		/// @see SetPosition, Move
		inline const sf::Vector2f & GetPosition() const { return transformable.getPosition(); }

		/// @Get the (relative) rotation
		/// @returns Current rotation in degrees
		/// @see SetRotation, Rotate
		inline float GetRotation() const { return transformable.getRotation(); }

		/// @brief Get the (relative) scale factors
		/// @returns Current scale factor
		/// @see SetScaleFactor
		inline const sf::Vector2f & GetScale() const { return transformable.getScale(); }

		/// @Get the local origin
		/// @returns the current origin
		/// @see SetOrigin
		inline const sf::Vector2f & GetOrigin() const { return transformable.getOrigin(); }

		/// @brief Move the entity's transform component by a given offset
		/// @details This method adds to the current position of the object, unlike SetPosition which overwrites it.
		/// Thus, it is equivalent to the following code:
		///
		/// @code
		/// auto position = entity.GetComponent<mbe::TransformComponent>().GetPosition();
		/// entity.GetComponent<mbe::TransformComponent>().SetPosition(position.x + offsetX, position.y + offsetY);
		/// @endcode
		///
		/// @param offset The vector by which the entity is moved
		/// @see SetPosition, GetPosition
		inline void Move(const sf::Vector2f & offset) { transformable.move(offset); }

		/// @brief Rotate the entity's transform component
		/// @details This function adds to the current rotation of the object unlike setRotation which overwrites it.
		/// Thus, it is equivalent to the following code:
		///
		/// @code
		/// entity.GetComponent<mbe::TransformComponent>().SetRotation(entity.GetComponent<mbe::TransformComponent>().GetRotation() + angle);
		/// @endcode
		///
		/// @param angle Angle of rotation, in degrees
		/// @see SetRotation, GetRotation
		inline void Rotate(float angle) { transformable.rotate(angle); }

		/// @brief Scale the entity's transform component
		/// @details This function multiplies the current scale of the object unlike setScale which overwrites it.
		/// Thus, it is equivalent to the following code:
		///
		/// @code
		/// auto scale = entity.GetComponent<mbe::TransformComponent>().GetScale();
		/// entity.GetComponent<mbe::TransformComponent>().SetScale(scale.x * factor.x, scale.y * factor.y);
		/// @endcode
		///
		/// @param factor Scale factors
		/// @see SetScale, GetScale
		inline void Scale(const sf::Vector2f & factor) { transformable.scale(factor); }

		/// @brief Get the underlying sf::Transformable
		/// @returns A reference to the underlying sf::Transformable
		/// @see GetLocalTransform, GetWorldTransform, GetInverseTransform
		inline const sf::Transformable & GetTransformable() const { return transformable; }

		/// @brief Get the local transform
		/// @details To get the actual transform, use GetWorldTransform()
		/// @see GetWorldTransform, GetInverseTransform, GetTransformable
		inline const sf::Transform & GetLocalTransform() const { return transformable.getTransform(); }

		/// @brief Get the inverse local transform
		/// @returns The inverse of the local transform of the entity
		/// @see GetLocalTransform, GetWorldTransform, GetTransformable
		inline const sf::Transform & GetInverseTransform() const { return transformable.getInverseTransform(); }

		/// @brief Get the actual position
		/// @details To get the local position, use GetPosition(). The position is calculated by internally calling GetWorldTransform().
		/// @returns The absolute position of this entity
		/// @see GetPosiion, SetPosition, Move
		// no const & since the value is calculates within the function
		sf::Vector2f GetWorldPosition() const;

		/// @brief Get the actual transform
		/// @details To get the local transform, use GetLocalTransform().
		/// Accumulates all the relative transforms until reaching the root i.e. an entity whose parent transform entiy is null.
		/// @returns The absolute transform of this entity
		/// @see GetLocalTransform, GetInverseTransform, GetTransformaable
		// no const & since the value is calculates within the function
		sf::Transform GetWorldTransform() const;

	private:
		sf::Transformable transformable;
	};

} // namespace mbe