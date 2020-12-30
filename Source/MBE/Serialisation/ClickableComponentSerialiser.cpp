#include <MBE/Serialisation/ClickableComponentSerialiser.h>

#include <MBE/Core/TinyXML.h>
#include <MBE/Input/ClickableComponent.h>

using namespace mbe;

void ClickableComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	auto& clickableComponent = entity.AddComponent<ClickableComponent>();

	// Get absorbe click
	const auto absorbeClickElement = componentData.FirstChildElement("AbsorbeClick");
	if (absorbeClickElement != nullptr)
	{
		bool absorbeClick = false;
		if (absorbeClickElement->QueryBoolText(&absorbeClick) != XML_SUCCESS)
			throw std::runtime_error("Load clickable component: Failed to parese AbsorbeClick text");

		// Set absorbe click
		clickableComponent.SetAbsorbeClick(absorbeClick);
	}

	// Get bubble up
	const auto bubbleUpElement = componentData.FirstChildElement("BubbleUp");
	if (bubbleUpElement != nullptr)
	{
		bool bubbleUp = false;
		if (bubbleUpElement->QueryBoolText(&bubbleUp) != XML_SUCCESS)
			throw std::runtime_error("Load clickable component: Failed to parese BubbleUp text");

		// Set bubble up
		clickableComponent.SetBubbleUp(bubbleUp);
	}
}

void ClickableComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have a clickable component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<ClickableComponent>() == false)
		throw std::runtime_error("Store clickable component: The entity must have an mbe::ClickableComponent");
	const auto& clickableComponent = entity.GetComponent<mbe::ClickableComponent>();

	// Store AbsorbeClick
	auto absorbeClickElement = document.NewElement("AbsorbeClick");
	absorbeClickElement->SetText(clickableComponent.IsClickAbsorebd());
	componentData.InsertEndChild(absorbeClickElement);

	// Store BubbleUp
	auto bubbleUpElement = document.NewElement("BubbleUp");
	bubbleUpElement->SetText(clickableComponent.DoesBubbleUp());
	componentData.InsertEndChild(bubbleUpElement);
}
