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
		/// @brief Enables an entity to use the components constructor
		/// @details Making the constructor protected is necessarry to ensure that the
		/// mbe::Entity class (mbe::Entity::CreateComponentis the only place where components can be created.
		friend class Entity;

	public:
		typedef std::shared_ptr<Component> Ptr;
		typedef std::weak_ptr<Component> WPtr;
		typedef std::unique_ptr<Component> UPtr;

	protected:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		Component(Entity & parentEntity);

	public:
		/// @brief Default destructor
		virtual ~Component() = default;

		/// @brief Virtual function to update the component that should be called every frame
		/// @detail This method can but does not have to be overwritten in derived components
		/// and is guaranteed to be called once each frame.
		/// @param frameTime The delta time between two successive frames
		virtual void Update(sf::Time frameTime) {};

		/// @brief Gets the entity to which this component belongs
		/// @returns A reference to its parent Enity
		Entity & GetParentEntity() const { return parentEntity; }

	protected:
		Entity & parentEntity;
	};
}
