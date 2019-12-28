#include "..\..\..\Include\MBE\Graphics\TextureWrapperComponent.h"
#include <MBE/Graphics/TextureWrapperComponent.h>

using namespace mbe;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<mbe::TextureWrapperComponent>;

TextureWrapperComponent::TextureWrapperComponent(EventManager & eventManager, Entity & parentEntity, const TextureWrapperHolder<std::string>& textureWrapperHolder, const std::string& textureWrapperName) :
	Component(eventManager, parentEntity),
	textureWrapperHolder(textureWrapperHolder),
	textureWrapperName(textureWrapperName),
	textureRect({ 0, 0 }, static_cast<sf::Vector2i>(GetTextureWrapper().GetTexture().getSize()))
{
}

void TextureWrapperComponent::SetTextureWrapper(const std::string& textureWrapperName, bool resetTextureRect)
{
	// Recompute the texture rect if required
	if (resetTextureRect)
		textureRect = { {0, 0}, {static_cast<sf::Vector2i>(GetTextureWrapper().GetTexture().getSize())} };

	if (this->textureWrapperName == textureWrapperName)
		return;

	// Assign the new texture wrapper
	this->textureWrapperName = textureWrapperName;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureWrapper)));
}

void TextureWrapperComponent::SetTextureRect(const sf::IntRect & textureRect)
{
	// If the current texture rect is the same as the new one
	if (textureRect == this->textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	assert(textureRect.left + textureRect.width <= GetTextureWrapper().GetTexture().getSize().x
		|| textureRect.top + textureRect.height <= GetTextureWrapper().GetTexture().getSize().y
		&& "TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	this->textureRect = textureRect;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

void TextureWrapperComponent::SetTextureRect(sf::IntRect && textureRect)
{
	// If the current texture rect is the same as the new one
	if (textureRect == this->textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	assert(textureRect.left + textureRect.width <= GetTextureWrapper().GetTexture().getSize().x
		|| textureRect.top + textureRect.height <= GetTextureWrapper().GetTexture().getSize().y
		&& "TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	this->textureRect = std::move(textureRect);

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

const TextureWrapper& TextureWrapperComponent::GetTextureWrapper() const
{
	return textureWrapperHolder[textureWrapperName];
}


