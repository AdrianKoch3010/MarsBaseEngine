#pragma once

/// @file
/// @brief Class mbe::ClickableComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::ClicableComponent
	/// @details The default values are the same as in the mbe::ClickableComponent
	/// @n@n **XML format**
	/// @code
	/// <Component type="ClickableComponent>
	///		<!-- Optional - default: true -->
	///		<AbsorbeClick>bool</AbsorbeClick>
	///		<!-- Optional - default: true -->
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