#pragma once

/// @file
/// @brief Class mbe::ClickableComponentSerialser

#include <MBE/Core/TinyXML.h>
#include <MBE/Core/Entity.h>

namespace mbe
{

	/// @brief Static class providing functions for loading and storing the clickable component data
	class ClickableComponentSerialser
	{
	public:

		struct Data
		{
			bool absorbeClick = false;
			bool bubbleUp = false;
		};

	public:
		/// @brief Default constructor
		ClickableComponentSerialser() = default;

		/// @brief Default Destructor
		~ClickableComponentSerialser() = default;

	public:
		static Data LoadComponent(const tinyxml2::XMLElement& componentData);

		static void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData);
	};

} // namespace mbe