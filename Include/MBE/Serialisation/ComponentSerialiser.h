#pragma once

/// @file
/// @brief class mbe::ComponentSerialiser

#include <memory>

// TinyXML2
#include <MBE/Core/TinyXML.h>

#include <MBE/Core/Entity.h>
#include <MBE/Core/EntityManager.h>

namespace mbe
{

	/// @brief Interface for loading and storing component data from and to an XML file format
	class ComponentSerialser
	{
	public:
		typedef std::shared_ptr<ComponentSerialser> Ptr;
		typedef std::weak_ptr<ComponentSerialser> WPtr;
		typedef std::unique_ptr<ComponentSerialser> UPtr;

	public:
		/// @brief Default constructor
		ComponentSerialser() = default;

		/// @brief Virtual default destructor
		virtual ~ComponentSerialser() = default;

	public:
		/// @brief Interface for loading and adding a component to the entity
		/// @param entity The entity to which the component is added
		/// @param componentData The XML element which is parsed and used to determine the value of the component
		virtual void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) = 0;

		/// @brief Interface for serialising a component into an XML element
		/// @param entity The entity whos component is stored
		/// @param document The xml document to which the component data is stored to
		/// @param componentData The xml element to which the component data is stored to
		virtual void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) = 0;
	};


} // namespace mbe