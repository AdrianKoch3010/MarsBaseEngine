#pragma once

/// @file
/// @brief Class mbe::PixelMaskClickableComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Serialisation/ClickableComponentSerialiser.h>
#include <MBE/Core/AssetHolder.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::PixelMaskClicableComponent
	/// @details **XML format**
	/// @code
	/// <Component type="PixelMaskClickableComponent>
	///		<AbsorbeClick>bool</AbsorbeClick>
	///		<BubbleUp>bool</BubbleUp>
	/// </Component>
	/// @endcode
	class PixelMaskClickableComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<PixelMaskClickableComponentSerialiser> Ptr;
		typedef std::weak_ptr<PixelMaskClickableComponentSerialiser> WPtr;
		typedef std::unique_ptr<PixelMaskClickableComponentSerialiser> UPtr;

	public:
		/// @brief Default constructor
		PixelMaskClickableComponentSerialiser() = default;

		/// @brief Default destructor
		~PixelMaskClickableComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;
	};

} // namespace mbe