#pragma once

/// @file
/// @brief class mbe::TopDownInformationComponentSerialiser

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{
	/// @brief Component serialiser for the mbe::TopDownInformationComponent
	/// @details XML format
	/// @code
	/// <Component type="TopDownInformationComponent">
	///		<LogicalBottomOffset>float</LogicalBottomOffset>
	/// </Component>
	/// @endcode
	class TopDownInformationComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TopDownInformationComponentSerialiser> Ptr;
		typedef std::weak_ptr<TopDownInformationComponentSerialiser> WPtr;
		typedef std::unique_ptr<TopDownInformationComponentSerialiser> UPtr;

	public:
		TopDownInformationComponentSerialiser() = default;
		~TopDownInformationComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;
	};

} // namespace mbe