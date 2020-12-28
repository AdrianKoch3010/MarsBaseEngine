#pragma once

/// @file
/// @brief class mbe::ComponentSerialiser

#include <memory>

// TinyXML2
#include <MBE/Core/TinyXML.h>

#include <MBE/Core/Utility.h>
#include <MBE/Core/Entity.h>
#include <MBE/Core/EntityManager.h>

MBE_DECLARE_TYPE_ID(ComponentSerialiser)

namespace mbe
{
	/// @brief Interface for loading and storing component data from and to an XML file format
	class ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<ComponentSerialiser> Ptr;
		typedef std::weak_ptr<ComponentSerialiser> WPtr;
		typedef std::unique_ptr<ComponentSerialiser> UPtr;

		typedef detail::ComponentSerialiserTypeID TypeID;

	public:
		/// @brief Default constructor
		ComponentSerialiser() = default;

		/// @brief Virtual default destructor
		virtual ~ComponentSerialiser() = default;

	public:
		/// @brief Interface for loading and adding a component to the entity
		/// @param entity The entity to which the component is added
		/// @param componentData The XML element which is parsed and used to determine the value of the component
		virtual void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const = 0;

		/// @brief Interface for serialising a component into an XML element
		/// @param entity The entity whos component is stored
		/// @param document The XML document to which the component data is stored to
		/// @param componentData The XML element to which the component data is stored to
		virtual void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const = 0;

		MBE_GET_TYPE_ID(ComponentSerialiser)
	};

} // namespace mbe