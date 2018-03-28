#pragma once

/// @file
/// @brief Abstract class mbe::Component

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace mbe
{

	class Entity;

	/// @brief The base class for all components
	/// @details Components can be added to a mbe::Entity. They encapsulate one specific functionality
	/// and provide an interface for accessing that functionality through the mbe::Entity::GetComponent() function.
	/// A component may require its parent entity to have/provide one or more other components of a specific type.
	/// @warning Components should only be created using the mbe::Entity::AddComponent() function. Otherwise,
	/// it cannot be guaranteed that their Update() methods will be called correctly
	class Component : private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<Component> Ptr;
		typedef std::weak_ptr<Component> WPtr;
		typedef std::unique_ptr<Component> UPtr;

	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		Component(Entity & parentEntity);

		/// @brief Default destructor
		virtual ~Component() = default;

		/// @brief Purely virtual function to update the component that should be called every frame
		/// @param frameTime The delta time between two successive frames
		virtual void Update(sf::Time frameTime) = 0;

		/// @brief Gets the entity to which this component belongs
		/// @returns A reference to its parent Enity
		Entity & GetParentEntity() const { return parentEntity; }

	protected:
		Entity & parentEntity;
	};
}
