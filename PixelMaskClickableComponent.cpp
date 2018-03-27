#include "PixelMaskClickableComponent.h"

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(PixelMaskClickableComponent, ClickableComponent)

PixelMaskClickableComponent::PixelMaskClickableComponent(Entity & parentEntity) :
	ClickableComponent(parentEntity)
{
	// This requires the mbe::TexturedRenderComponent to be added first
	assert(parentEntity.HasComponent<TexturedRenderComponent>() && "PixelMaskClickableComponent: The parent entity must have a mbe::TexturedClickableComponent");
}

bool PixelMaskClickableComponent::Contains(const sf::Vector2f & position) const
{
	const auto & texturedRenderComponent = parentEntity.GetComponent<TexturedRenderComponent>();
	const auto pixelMaskPtr = texturedRenderComponent.GetTextureWrapper().GetPixelMask();

	// What to do when no pixelMask has been created?
	assert(pixelMaskPtr != nullptr && "PixelMaskClickableComponent: The mbe::PixelMask must have been created");

	// Get the texture rect
	const auto & textureRect = texturedRenderComponent.GetTextureRect();

	return pixelMaskPtr->Contains(position, textureRect);
}