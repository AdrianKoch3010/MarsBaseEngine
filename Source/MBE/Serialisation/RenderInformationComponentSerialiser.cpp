#include <cassert>

#include <MBE/Graphics/RenderInformationComponent.h>

#include <MBE/Serialisation/RenderInformationComponentSerialiser.h>

using namespace mbe;

void RenderInformationComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	auto& renderInformationComponent = entity.AddComponent<RenderInformationComponent>();

	// Get the render layer
	const auto renderLayerElement = componentData.FirstChildElement("RenderLayer");
	if (renderLayerElement == nullptr)
		throw ParseError(MBE_NAME_OF(RenderInformationComponentSerialiser), "RenderLayer node is required", componentData.GetLineNum());
	auto renderLayerText = renderLayerElement->GetText();
	if (renderLayerText == nullptr)
		throw ParseError(MBE_NAME_OF(RenderInformationComponentSerialiser), "Failed to parse RenderLayer text", renderLayerElement->GetLineNum());
	std::string renderLayerString{ renderLayerText };
	renderInformationComponent.SetRenderLayer(StringToRenderLayer(renderLayerString));

	// Get the z order
	const auto zOrderElement = componentData.FirstChildElement("ZOrder");
	if (zOrderElement != nullptr)
	{
		float zOrder;
		if (zOrderElement->QueryFloatText(&zOrder) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(RenderInformationComponentSerialiser), "Failed to parse ZOrder text", zOrderElement->GetLineNum());
		renderInformationComponent.SetZOrder(zOrder);
	}
}

void RenderInformationComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have a render information component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<RenderInformationComponent>() == false)
		throw std::runtime_error("Store render information component: The entity must have an mbe::RenderInformationComponent");
	const auto& renderInformationComponent = entity.GetComponent<mbe::RenderInformationComponent>();

	// Store the render layer
	auto renderLayerElement = document.NewElement("RenderLayer");
	renderLayerElement->SetText(RenderLayerToString(renderInformationComponent.GetRenderLayer()).c_str());
	componentData.InsertEndChild(renderLayerElement);

	// Store the z order
	auto zOrderElement = document.NewElement("ZOrder");
	zOrderElement->SetText(renderInformationComponent.GetZOrder());
	componentData.InsertEndChild(zOrderElement);
}

std::string RenderInformationComponentSerialiser::RenderLayerToString(RenderLayer renderLayer)
{
	try
	{
		return renderLayerStringDictionary.at(renderLayer);
	}
	catch (const std::out_of_range& error)
	{
		// Forgot to add the render layer to the renderLayerString dictionary
		assert(false && "RenderLayerToString: Unkown render layer");
	}

	// Default return
	return std::string();
}

RenderLayer RenderInformationComponentSerialiser::StringToRenderLayer(const std::string& renderLayerString)
{
	// Linear search for the string (slow but consistent)
	for (auto it = renderLayerStringDictionary.begin(); it != renderLayerStringDictionary.end(); ++it)
	{
		if (it->second == renderLayerString)
			return it->first;
	}

	throw std::runtime_error("StringToRenderLayer: Unknown render layer string");

	// Return default for consistency sake
	return RenderLayer::LayerCount;
}
