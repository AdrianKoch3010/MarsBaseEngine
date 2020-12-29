#pragma once

/// @file
/// @brief Abstract class mbe::Component

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/ComponentValueChangedEvent.h>

namespace mbe
{

	namespace detail
	{
		/// @brief Defines the type of the component id
			/// @details This id is generated at runtime for each component class by calling mbe::detail::GetComponentTypeID()
		typedef std::size_t ComponentTypeID;

		/// @brief Returns a unique ComponentTypeID (for each function call)
		inline ComponentTypeID GetComponentID() noexcept
		{
			// This will only be initialised once
			static ComponentTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		/// @brief Returns a unique number (of type std::size_t) for each type T
		/// @details Each component type will have its own unique id.
		/// The id will be the same for every instance of that type
		/// @tparam TComponent The type for which the id is generated. It must inherit from mbe::Component
		template <typename TComponent>
		inline ComponentTypeID GetComponentTypeID() noexcept
		{
			// make sure that TComponentSerialiser inherits from ComponentSerialiser
			static_assert(std::is_base_of<Component, TComponent>::value, "The component must inherit from mbe::Component");

			// There will be only one static variable for each template type
			static ComponentTypeID typeId = GetComponentID();
			return typeId;
		}
	} // namespace detail

	class Entity;

	/// @brief The base class for all components
	/// @details Components can be added to a mbe::Entity. They encapsulate one specific functionality
	/// and provide an interface for accessing that functionality through the mbe::Entity::GetComponent() function.
	/// A component may require its parent entity to have/provide one or more other components of a specific type.
	/// @note Components should only be created using the mbe::Entity::AddComponent() function.
	class Component : private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<Component> Ptr;
		typedef std::weak_ptr<Component> WPtr;
		typedef std::unique_ptr<Component> UPtr;

		typedef detail::ComponentTypeID TypeID;

	public:
		/// @brief Enables an entity to use the components constructor.
		/// @details Making the constructor protected is necessarry to ensure
		/// that the mbe::Entity class (mbe::Entity::CreateComponentis the only place where components can be created.
		friend class Entity;

	protected:
		/// @brief Protected Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		Component(EventManager& eventManager, Entity& parentEntity);

	public:
		/// @brief Default destructor
		virtual ~Component() = default;

		/// @brief Gets the entity to which this component belongs
		/// @returns A reference to its parent Enity
		Entity& GetParentEntity() const { return parentEntity; }

		/// @brief Generates the type id for a component
		/// @tparam TComponent The type of component. Must inherit from mbe::Component
		/// @return The type id
		template<class TComponent>
		static TypeID GetTypeID();

	protected:
		EventManager& eventManager;
		Entity& parentEntity;
	};


#pragma region Template Implementations

	template<class TComponent>
	inline Component::TypeID Component::GetTypeID()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must inherit from mbe::Component");
		return detail::GetComponentTypeID<TComponent>();
	}

#pragma endregion



} // namespace mbe
