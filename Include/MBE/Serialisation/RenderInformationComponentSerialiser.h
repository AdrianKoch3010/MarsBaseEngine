#pragma once

/// @file
/// @brief class mbe::RenderInformationComponentSerialiser

#include <memory>
#include <string>
#include <unordered_map>

#include <MBE/Graphics/RenderInformationComponent.h>
#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::RenderInformationComponent
	/// @details XML formal
	/// @code
	/// <Component type="RenderInformationComponent">
	///		<RenderLayer>string</RenderLayer>
	///		<ZOrder>float</ZOrder>
	/// </Component>
	/// @endcode
	class RenderInformationComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<RenderInformationComponentSerialiser> Ptr;
		typedef std::weak_ptr<RenderInformationComponentSerialiser> WPtr;
		typedef std::unique_ptr<RenderInformationComponentSerialiser> UPtr;

	public:
		RenderInformationComponentSerialiser() = default;
		~RenderInformationComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;

	public:
		static std::string RenderLayerToString(RenderLayer renderLayer);

		// Throws if the string doesn't correspond to a render layer
		static RenderLayer StringToRenderLayer(const std::string& renderLayerString);
	};

	static const std::unordered_map<RenderLayer, std::string> renderLayerStringDictionary = {
	{ RenderLayer::Background, "Background" },
{ RenderLayer::Foreground, "Foreground" },
{ RenderLayer::Objects, "Objects" },
{ RenderLayer::GUI, "GUI" },
{ RenderLayer::LayerCount, "Undefined" }
	};


} // namespace mbe