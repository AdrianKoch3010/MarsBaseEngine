#pragma once

/// @file
/// @brief Class CustomRenderComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::CustomRenderComponent
	/// @details @n@n **XML format**
	/// This component doesn't require storing any state as its data is populated by the respective systems
	/// @code
	/// <Component type="CustomRenderComponent" />
	/// @endcode
	class CustomRenderComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<CustomRenderComponentSerialiser> Ptr;
		typedef std::weak_ptr<CustomRenderComponentSerialiser> Wptr;
		typedef std::unique_ptr<CustomRenderComponentSerialiser> UPtr;

	public:
		CustomRenderComponentSerialiser() = default;
		~CustomRenderComponentSerialiser() = default;

	public:
		/// @brief Loads and adds a custom render component to the entity
		/// @param entity The entity to which the component is added
		/// @param componentData The XML element which is parsed and used to determine the value of the component
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		/// @brief Serialises a custom render component into an XML element
		/// @param entity The entity whos component is stored
		/// @param document The XML document to which the component data is stored to
		/// @param componentData The XML element to which the component data is stored to
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;
	};

} // namespace mbe