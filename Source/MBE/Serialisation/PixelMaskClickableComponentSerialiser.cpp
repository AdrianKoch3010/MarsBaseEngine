#include <MBE/Serialisation/ClickableComponentSerialiser.h>
#include <MBE/Input/PixelMaskClickableComponent.h>

#include <MBE/Serialisation/PixelMaskClickableComponentSerialiser.h>

using namespace mbe;

void PixelMaskClickableComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	// Get the component data
	auto data = ClickableComponentSerialser::LoadComponent(componentData);

	// Add the component and set the data
	auto& pixelMaskClickableComponent = entity.AddComponent<PixelMaskClickableComponent>();
	pixelMaskClickableComponent.SetBubbleUp(data.bubbleUp);
	pixelMaskClickableComponent.SetAbsorbeClick(data.absorbeClick);
}

void PixelMaskClickableComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// Delegate to the cliickable component serialser
	ClickableComponentSerialser::StoreComponent(entity, document, componentData);
}
