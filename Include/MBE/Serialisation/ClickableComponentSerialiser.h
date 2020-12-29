//#pragma once
//
///// @file
///// @brief Class mbe::ClickableComponentSerialser
//
//#include <MBE/Core/TinyXML.h>
//#include <MBE/Core/Entity.h>
//
//namespace mbe
//{
//
//	/// @brief Static class providing functions for loading and storing the clickable component data
//	class ClickableComponentSerialser
//	{
//	public:
//
//		struct Data
//		{
//			bool absorbeClick = false;
//			bool bubbleUp = false;
//		};
//
//	public:
//		/// @brief Default constructor
//		ClickableComponentSerialser() = default;
//
//		/// @brief Default Destructor
//		~ClickableComponentSerialser() = default;
//
//	public:
//		static Data LoadComponent(const tinyxml2::XMLElement& componentData);
//
//		static void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData);
//	};
//
//} // namespace mbe


#pragma once

/// @file
/// @brief Class mbe::ClickableComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::PixelMaskClicableComponent
	/// @details **XML format**
	/// @code
	/// <Component type="ClickableComponent>
	///		<AbsorbeClick>bool</AbsorbeClick>
	///		<BubbleUp>bool</BubbleUp>
	/// </Component>
	/// @endcode
	class ClickableComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<ClickableComponentSerialiser> Ptr;
		typedef std::weak_ptr<ClickableComponentSerialiser> WPtr;
		typedef std::unique_ptr<ClickableComponentSerialiser> UPtr;

	public:
		/// @brief Default constructor
		ClickableComponentSerialiser() = default;

		/// @brief Default destructor
		~ClickableComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;
	};

} // namespace mbe